#pragma once
#include <QAbstractItemModel>
#include <QVector>

#include "../../tree/TreeItem.hpp"

class TreeModel : public QAbstractItemModel
{
public:
    explicit TreeModel(const QVector<NodeData> &nodes, QObject *parent = nullptr);

public:
    QModelIndex index(
        int row,
        int column,
        const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(
        const QModelIndex &index,
        int role = Qt::DisplayRole) const override;

    QVariant headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;

    void reset();

public:
    void nodeAboutToBeAdded(int row);
    void nodeAdded();

    void nodeAboutToBeRemoved(int row);
    void nodeRemoved();

    void leafAboutToBeAdded(int nodeRow, int leafRow);
    void leafAdded();

    void leafAboutToBeRemoved(int nodeRow, int leafRow);
    void leafRemoved();

    void leafChanged(int nodeRow, int leafRow);

private:
    static quintptr makeNodeId(int nodeId);
    static quintptr makeLeafId(int leafId);

    static bool isLeafId(quintptr id);
    static int extractId(quintptr id);

    static constexpr quintptr LEAF_FLAG = quintptr(1);

private:
    const QVector<NodeData> &nodes_;
};