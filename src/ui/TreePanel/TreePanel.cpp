#include "TreePanel.hpp"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QTreeView>
#include <QVBoxLayout>

TreePanel::TreePanel(QWidget *parent)
    : QWidget(parent)
{
    treeView_ = new QTreeView(this);
    treeModel_ = new TreeModel(treeView_);

    treeView_->setModel(treeModel_);
    treeView_->setAlternatingRowColors(true);
    treeView_->setUniformRowHeights(true);
    treeView_->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView_->setSelectionBehavior(QAbstractItemView::SelectRows);

    treeView_->header()->setStretchLastSection(true);
    treeView_->setColumnWidth(0, 300);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(treeView_);
}

QTreeView *TreePanel::treeView() const
{
    return treeView_;
}

void TreePanel::setNodes(const QVector<NodeData> &nodes)
{
    treeModel_->setNodes(nodes);
    treeView_->expandAll();
}

QModelIndex TreePanel::currentIndex() const
{
    return treeView_->currentIndex();
}

void TreePanel::addNode(const NodeData &nodeData)
{
    treeModel_->addNode(nodeData);
}

void TreePanel::removeNode(int row)
{
    treeModel_->removeNode(row);
}

void TreePanel::addLeaf(int nodeRow, const LeafData &leafData)
{
    treeModel_->addLeaf(nodeRow, leafData);
}

void TreePanel::removeLeaf(int nodeRow, int leafRow)
{
    treeModel_->removeLeaf(nodeRow, leafRow);
}

void TreePanel::updateLeaf(
    int nodeRow,
    int leafRow,
    const LeafData &leafData)
{
    treeModel_->updateLeaf(nodeRow, leafRow, leafData);
}