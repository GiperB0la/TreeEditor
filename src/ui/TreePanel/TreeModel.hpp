#pragma once
#include <QAbstractItemModel>
#include <QVector>

#include "../../tree/TreeItem.hpp"

class TreeModel : public QAbstractItemModel
{
public:
    explicit TreeModel(QObject *parent = nullptr);

public:
    void setNodes(const QVector<NodeData> &nodes);

    QModelIndex index(
        int row,
        int column,
        const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(
        const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(
        const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(
        const QModelIndex &index,
        int role = Qt::DisplayRole) const override;

    QVariant headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;

    void addNode(const NodeData &node);
    void removeNode(int row);
    void addLeaf(int nodeRow, const LeafData &leaf);
    void removeLeaf(int nodeRow, int leafRow);
    void updateLeaf(int nodeRow, int leafRow, const LeafData& leaf);

private:
    QVector<NodeData> nodes_;
};