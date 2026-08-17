#include "TreeManager.hpp"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <utility>

TreeManager::TreeManager(TreeRepository &repository)
    : repository_(repository)
{
}

bool TreeManager::load()
{
    QVector<NodeData> nodes;

    if (!repository_.loadTree(nodes)) 
    {
        lastError_ = repository_.lastError();
        return false;
    }

    nodes_ = std::move(nodes);

    lastError_.clear();

    return true;
}

const QVector<NodeData> &TreeManager::nodes() const
{
    return nodes_;
}

QString TreeManager::lastError() const
{
    return lastError_;
}

bool TreeManager::addNode(const QString &name)
{
    int nodeId = 0;

    if (!repository_.addNode(name, nodeId)) 
    {
        lastError_ = repository_.lastError();
        return false;
    }

    NodeData node;
    node.id = nodeId;
    node.name = name;

    nodes_.append(node);

    lastError_.clear();

    return true;
}

bool TreeManager::deleteNode(int nodeId)
{
    for (int i = 0; i < nodes_.size(); ++i) 
    {
        if (nodes_[i].id != nodeId) 
        {
            continue;
        }

        if (!repository_.deleteNode(nodeId)) 
        {
            lastError_ = repository_.lastError();
            return false;
        }

        nodes_.removeAt(i);

        lastError_.clear();

        return true;
    }

    lastError_ = "Узел не найден";

    return false;
}

bool TreeManager::addLeaf(
    int nodeId,
    const QString &name,
    double value)
{
    for (NodeData &node : nodes_) 
    {
        if (node.id != nodeId) 
        {
            continue;
        }

        int leafId = 0;

        if (!repository_.addLeaf(
            nodeId,
            name,
            value,
            leafId)) 
        {
            lastError_ = repository_.lastError();
            return false;
        }

        LeafData leaf;
        leaf.id = leafId;
        leaf.nodeId = nodeId;
        leaf.name = name;
        leaf.value = value;

        node.leaves.append(leaf);

        lastError_.clear();

        return true;
    }

    lastError_ = "Узел не найден";

    return false;
}

bool TreeManager::deleteLeaf(int leafId)
{
    for (NodeData &node : nodes_) 
    {
        for (int i = 0; i < node.leaves.size(); ++i) 
        {
            if (node.leaves[i].id != leafId) 
            {
                continue;
            }

            if (!repository_.deleteLeaf(leafId)) 
            {
                lastError_ = repository_.lastError();
                return false;
            }

            node.leaves.removeAt(i);

            lastError_.clear();

            return true;
        }
    }

    lastError_ = "Лист не найден";

    return false;
}

bool TreeManager::updateLeaf(
    int leafId,
    const QString &name,
    double value)
{
    for (NodeData &node : nodes_)
    {
        for (LeafData &leaf : node.leaves)
        {
            if (leaf.id != leafId)
            {
                continue;
            }

            if (!repository_.updateLeaf(
                leafId,
                name,
                value))
            {
                lastError_ = repository_.lastError();
                return false;
            }

            leaf.name = name;
            leaf.value = value;

            lastError_.clear();

            return true;
        }
    }

    lastError_ = "Лист не найден";

    return false;
}

QVector<NodeData> TreeManager::filter(
    const std::function<bool(const LeafData&)> &predicate) const
{
    QVector<NodeData> result;

    for (const NodeData &node : nodes_)
    {
        NodeData filteredNode;
        filteredNode.id = node.id;
        filteredNode.name = node.name;

        for (const LeafData &leaf : node.leaves)
        {
            if (predicate(leaf))
            {
                filteredNode.leaves.append(leaf);
            }
        }

        if (!filteredNode.leaves.isEmpty())
        {
            result.append(filteredNode);
        }
    }

    return result;
}

QVector<NodeData> TreeManager::filter(
    const QString &nameText,
    const QString &valueText) const
{
    return filter(
        [&nameText, &valueText](const LeafData &leaf)
        {
            const bool nameMatches =
                nameText.isEmpty() ||
                leaf.name.contains(nameText, Qt::CaseInsensitive);

            const QString value = QString::number(leaf.value, 'g', 15);

            const bool valueMatches =
                valueText.isEmpty() ||
                value.contains(valueText);

            return nameMatches && valueMatches;
        });
}

int TreeManager::nodeCount() const
{
    return nodes_.size();
}

int TreeManager::leafCount() const
{
    int count = 0;

    for (const NodeData &node : nodes_)
    {
        count += node.leaves.size();
    }

    return count;
}

double TreeManager::minimumValue() const
{
    if (nodes_.isEmpty())
    {
        return 0.0;
    }

    bool found = false;
    double minimum = 0.0;

    for (const NodeData &node : nodes_)
    {
        for (const LeafData &leaf : node.leaves)
        {
            if (!found || leaf.value < minimum)
            {
                minimum = leaf.value;
                found = true;
            }
        }
    }

    return found ? minimum : 0.0;
}

double TreeManager::maximumValue() const
{
    if (nodes_.isEmpty())
    {
        return 0.0;
    }

    bool found = false;
    double maximum = 0.0;

    for (const NodeData &node : nodes_)
    {
        for (const LeafData &leaf : node.leaves)
        {
            if (!found || leaf.value > maximum)
            {
                maximum = leaf.value;
                found = true;
            }
        }
    }

    return found ? maximum : 0.0;
}

bool TreeManager::exportJson(const QString &filePath) const
{
    QJsonArray nodes;

    for (const NodeData &node : nodes_)
    {
        QJsonObject nodeObject;
        nodeObject["id"] = node.id;
        nodeObject["name"] = node.name;

        QJsonArray leaves;

        for (const LeafData &leaf : node.leaves)
        {
            QJsonObject leafObject;
            leafObject["id"] = leaf.id;
            leafObject["nodeId"] = leaf.nodeId;
            leafObject["name"] = leaf.name;
            leafObject["value"] = leaf.value;

            leaves.append(leafObject);
        }

        nodeObject["leaves"] = leaves;
        nodes.append(nodeObject);
    }

    QJsonObject root;
    root["nodes"] = nodes;

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        lastError_ = file.errorString();
        return false;
    }

    if (file.write(QJsonDocument(root).toJson(QJsonDocument::Indented)) == -1)
    {
        lastError_ = file.errorString();
        return false;
    }

    lastError_.clear();

    return true;
}

bool TreeManager::importJson(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        lastError_ = file.errorString();
        return false;
    }

    QJsonParseError parseError;

    const QJsonDocument document =
        QJsonDocument::fromJson(file.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        lastError_ = parseError.errorString();
        return false;
    }

    if (!document.isObject())
    {
        lastError_ = "Корневой элемент JSON не является объектом";
        return false;
    }

    const QJsonObject root = document.object();
    const QJsonValue nodesValue = root["nodes"];

    if (!nodesValue.isArray())
    {
        lastError_ = "Поле \"nodes\" отсутствует или не является массивом";
        return false;
    }

    QVector<NodeData> nodes;

    for (const QJsonValue &nodeValue : nodesValue.toArray())
    {
        if (!nodeValue.isObject())
        {
            lastError_ = "Элемент nodes не является объектом";
            return false;
        }

        const QJsonObject nodeObject = nodeValue.toObject();

        NodeData node;
        node.id = nodeObject["id"].toInt();
        node.name = nodeObject["name"].toString();

        const QJsonValue leavesValue = nodeObject["leaves"];

        if (!leavesValue.isArray())
        {
            lastError_ = "Поле \"leaves\" отсутствует или не является массивом";
            return false;
        }

        for (const QJsonValue &leafValue : leavesValue.toArray())
        {
            if (!leafValue.isObject())
            {
                lastError_ = "Элемент leaves не является объектом";
                return false;
            }

            const QJsonObject leafObject = leafValue.toObject();

            LeafData leaf;
            leaf.id = leafObject["id"].toInt();
            leaf.nodeId = leafObject["nodeId"].toInt();
            leaf.name = leafObject["name"].toString();
            leaf.value = leafObject["value"].toDouble();

            node.leaves.append(leaf);
        }

        nodes.append(node);
    }

    if (!repository_.replaceTree(nodes))
    {
        lastError_ = repository_.lastError();
        return false;
    }

    nodes_ = std::move(nodes);
    lastError_.clear();

    return true;
}