#include "TreePanel.hpp"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QTreeView>
#include <QVBoxLayout>

#include "../../models/TreeModel.hpp"
#include "../../models/TreeFilterProxyModel.hpp"

TreePanel::TreePanel(TreeModel *model, QWidget *parent)
    : QWidget(parent)
{
    treeView_ = new QTreeView(this);

    proxyModel_ = new TreeFilterProxyModel(this);
    proxyModel_->setSourceModel(model);

    treeView_->setModel(proxyModel_);
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

QModelIndex TreePanel::currentIndex() const
{
    return treeView_->currentIndex();
}

void TreePanel::setNameFilter(const QString &text)
{
    proxyModel_->setNameFilter(text);
    treeView_->expandAll();
}

void TreePanel::setValueFilter(const QString &text)
{
    proxyModel_->setValueFilter(text);
    treeView_->expandAll();
}

void TreePanel::resetFilters()
{
    proxyModel_->setNameFilter({});
    proxyModel_->setValueFilter({});
    treeView_->expandAll();
}