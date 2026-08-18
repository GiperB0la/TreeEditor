#pragma once
#include <QWidget>

class QTreeView;
class TreeModel;
class TreeFilterProxyModel;

class TreePanel : public QWidget
{
public:
    explicit TreePanel(TreeModel *model, QWidget *parent = nullptr);

public:
    QTreeView *treeView() const;
    QModelIndex currentIndex() const;

    void setNameFilter(const QString &text);
    void setValueFilter(const QString &text);
    void resetFilters();

private:
    QTreeView *treeView_;
    TreeFilterProxyModel *proxyModel_;
};