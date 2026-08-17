#include "TreeModel.hpp"

#include <QVariant>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void TreeModel::setNodes(const QVector<NodeData> &nodes)
{
    beginResetModel();
    nodes_ = nodes;
    endResetModel();
}

QModelIndex TreeModel::index(
    int row,
    int column,
    const QModelIndex &parent) const
{
    if (row < 0 || column < 0 || column >= columnCount(parent))
    {
        return {};
    }

    if (!parent.isValid())
    {
        if (row >= nodes_.size())
        {
            return {};
        }

        return createIndex(
            row,
            column,
            const_cast<NodeData *>(&nodes_[row]));
    }

    auto *node = static_cast<NodeData *>(parent.internalPointer());

    if (!node || parent.parent().isValid())
    {
        return {};
    }

    if (row >= node->leaves.size())
    {
        return {};
    }

    return createIndex(
        row,
        column,
        const_cast<LeafData *>(&node->leaves[row]));
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return {};
    }

    auto *ptr = index.internalPointer();

    for (int nodeIndex = 0; nodeIndex < nodes_.size(); ++nodeIndex)
    {
        NodeData *node = const_cast<NodeData *>(&nodes_[nodeIndex]);

        if (ptr == node)
        {
            return {};
        }

        for (int leafIndex = 0;
            leafIndex < node->leaves.size();
            ++leafIndex)
        {
            LeafData *leaf =
                const_cast<LeafData *>(&node->leaves[leafIndex]);

            if (ptr == leaf)
            {
                return createIndex(
                    nodeIndex,
                    0,
                    node);
            }
        }
    }

    return {};
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return nodes_.size();
    }

    if (parent.parent().isValid())
    {
        return 0;
    }

    auto *node = static_cast<NodeData *>(parent.internalPointer());

    if (!node)
    {
        return 0;
    }

    return node->leaves.size();
}

int TreeModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant TreeModel::data(
    const QModelIndex &index,
    int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return {};
    }

    auto *ptr = index.internalPointer();

    if (!ptr)
    {
        return {};
    }

    for (const NodeData &node : nodes_)
    {
        if (&node == ptr)
        {
            if (index.column() == 0)
            {
                return node.name;
            }

            return {};
        }

        for (const LeafData &leaf : node.leaves)
        {
            if (&leaf == ptr)
            {
                if (index.column() == 0)
                {
                    return leaf.name;
                }

                if (index.column() == 1)
                {
                    return leaf.value;
                }

                return {};
            }
        }
    }

    return {};
}

QVariant TreeModel::headerData(
    int section,
    Qt::Orientation orientation,
    int role) const
{
    if (orientation != Qt::Horizontal ||
        role != Qt::DisplayRole)
    {
        return {};
    }

    if (section == 0)
    {
        return "Название";
    }

    if (section == 1)
    {
        return "Значение";
    }

    return {};
}

void TreeModel::addNode(const NodeData &node)
{
    const int row = nodes_.size();

    beginInsertRows({}, row, row);

    nodes_.append(node);

    endInsertRows();
}

void TreeModel::removeNode(int row)
{
    if (row < 0 || row >= nodes_.size())
    {
        return;
    }

    beginRemoveRows({}, row, row);

    nodes_.removeAt(row);

    endRemoveRows();
}

void TreeModel::addLeaf(int nodeRow, const LeafData &leaf)
{
    if (nodeRow < 0 || nodeRow >= nodes_.size())
    {
        return;
    }

    NodeData &node = nodes_[nodeRow];

    const int row = node.leaves.size();
    const QModelIndex parentIndex = createIndex(
        nodeRow,
        0,
        &node);

    beginInsertRows(parentIndex, row, row);

    node.leaves.append(leaf);

    endInsertRows();
}

void TreeModel::removeLeaf(int nodeRow, int leafRow)
{
    if (nodeRow < 0 || nodeRow >= nodes_.size())
    {
        return;
    }

    NodeData &node = nodes_[nodeRow];

    if (leafRow < 0 || leafRow >= node.leaves.size())
    {
        return;
    }

    const QModelIndex parentIndex = createIndex(
        nodeRow,
        0,
        &node);

    beginRemoveRows(
        parentIndex,
        leafRow,
        leafRow);

    node.leaves.removeAt(leafRow);

    endRemoveRows();
}

void TreeModel::updateLeaf(
    int nodeRow,
    int leafRow,
    const LeafData &leaf)
{
    if (nodeRow < 0 || nodeRow >= nodes_.size())
    {
        return;
    }

    NodeData& node = nodes_[nodeRow];

    if (leafRow < 0 || leafRow >= node.leaves.size())
    {
        return;
    }

    beginResetModel();

    node.leaves[leafRow] = leaf;

    endResetModel();
}