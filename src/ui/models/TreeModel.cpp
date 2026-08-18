#include "TreeModel.hpp"

#include <QVariant>

TreeModel::TreeModel(
    const QVector<NodeData> &nodes,
    QObject *parent)
    : QAbstractItemModel(parent),
      nodes_(nodes)
{
}

quintptr TreeModel::makeNodeId(int nodeId)
{
    return static_cast<quintptr>(nodeId) << 1;
}

quintptr TreeModel::makeLeafId(int leafId)
{
    return (static_cast<quintptr>(leafId) << 1) | LEAF_FLAG;
}

bool TreeModel::isLeafId(quintptr id)
{
    return (id & LEAF_FLAG) != 0;
}

int TreeModel::extractId(quintptr id)
{
    return static_cast<int>(id >> 1);
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

        const NodeData &node = nodes_[row];

        return createIndex(
            row,
            column,
            makeNodeId(node.id));
    }

    if (parent.parent().isValid())
    {
        return {};
    }

    const int nodeId = extractId(parent.internalId());

    const NodeData *node = nullptr;

    for (const NodeData &item : nodes_)
    {
        if (item.id == nodeId)
        {
            node = &item;
            break;
        }
    }

    if (!node || row >= node->leaves.size())
    {
        return {};
    }

    const LeafData &leaf = node->leaves[row];

    return createIndex(
        row,
        column,
        makeLeafId(leaf.id));
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return {};
    }

    const quintptr id = index.internalId();

    if (!isLeafId(id))
    {
        return {};
    }

    const int leafId = extractId(id);

    for (int nodeRow = 0;
        nodeRow < nodes_.size();
        ++nodeRow)
    {
        const NodeData &node = nodes_[nodeRow];

        for (const LeafData &leaf : node.leaves)
        {
            if (leaf.id == leafId)
            {
                return createIndex(
                    nodeRow,
                    0,
                    makeNodeId(node.id));
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

    const int nodeId = extractId(parent.internalId());

    for (const NodeData &node : nodes_)
    {
        if (node.id == nodeId)
        {
            return node.leaves.size();
        }
    }

    return 0;
}

int TreeModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant TreeModel::data(
    const QModelIndex &index,
    int role) const
{
    if (!index.isValid() ||
        role != Qt::DisplayRole)
    {
        return {};
    }

    const quintptr id = index.internalId();
    const int objectId = extractId(id);

    if (!isLeafId(id))
    {
        for (const NodeData &node : nodes_)
        {
            if (node.id != objectId)
            {
                continue;
            }

            if (index.column() == 0)
            {
                return node.name;
            }

            return {};
        }

        return {};
    }

    for (const NodeData &node : nodes_)
    {
        for (const LeafData &leaf : node.leaves)
        {
            if (leaf.id != objectId)
            {
                continue;
            }

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

void TreeModel::reset()
{
    beginResetModel();
    endResetModel();
}

void TreeModel::nodeAboutToBeAdded(int row)
{
    beginInsertRows(
        QModelIndex(),
        row,
        row);
}

void TreeModel::nodeAdded()
{
    endInsertRows();
}

void TreeModel::nodeAboutToBeRemoved(int row)
{
    beginRemoveRows(
        QModelIndex(),
        row,
        row);
}

void TreeModel::nodeRemoved()
{
    endRemoveRows();
}

void TreeModel::leafAboutToBeAdded(int nodeRow, int leafRow)
{
    if (nodeRow < 0 || nodeRow >= nodes_.size())
    {
        return;
    }

    const NodeData &node = nodes_[nodeRow];

    const QModelIndex parentIndex = createIndex(
        nodeRow,
        0,
        makeNodeId(node.id));

    beginInsertRows(
        parentIndex,
        leafRow,
        leafRow);
}

void TreeModel::leafAdded()
{
    endInsertRows();
}

void TreeModel::leafAboutToBeRemoved(int nodeRow, int leafRow)
{
    if (nodeRow < 0 || nodeRow >= nodes_.size())
    {
        return;
    }

    const NodeData &node = nodes_[nodeRow];

    if (leafRow < 0 || leafRow >= node.leaves.size())
    {
        return;
    }

    const QModelIndex parentIndex = createIndex(
        nodeRow,
        0,
        makeNodeId(node.id));

    beginRemoveRows(
        parentIndex,
        leafRow,
        leafRow);
}

void TreeModel::leafRemoved()
{
    endRemoveRows();
}

void TreeModel::leafChanged(int nodeRow, int leafRow)
{
    if (nodeRow < 0 || nodeRow >= nodes_.size())
    {
        return;
    }

    const NodeData &node = nodes_[nodeRow];

    if (leafRow < 0 || leafRow >= node.leaves.size())
    {
        return;
    }

    const LeafData &leaf = node.leaves[leafRow];

    const QModelIndex nameIndex = createIndex(
        leafRow,
        0,
        makeLeafId(leaf.id));

    const QModelIndex valueIndex = createIndex(
        leafRow,
        1,
        makeLeafId(leaf.id));

    emit dataChanged(
        nameIndex,
        valueIndex,
        {Qt::DisplayRole});
}