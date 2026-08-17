#pragma once
#include <QWidget>

#include "TreeModel.hpp"

class QTreeView;

class TreePanel : public QWidget
{
public:
    explicit TreePanel(QWidget *parent = nullptr);

public:
    QTreeView *treeView() const;

    void setNodes(const QVector<NodeData> &nodes);
    QModelIndex currentIndex() const;

    void addNode(const NodeData &nodeData);
    void removeNode(int row);
    void addLeaf(int nodeRow, const LeafData &leafData);
    void removeLeaf(int nodeRow, int leafRow);
    void updateLeaf(
        int nodeRow,
        int leafRow,
        const LeafData &leafData);

private:
    QTreeView *treeView_ = nullptr;
    TreeModel *treeModel_ = nullptr;
};