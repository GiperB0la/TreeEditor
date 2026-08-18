#pragma once
#include <QSortFilterProxyModel>

class TreeFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit TreeFilterProxyModel(QObject *parent = nullptr);

public:
    void setNameFilter(const QString &text);
    void setValueFilter(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString nameFilter_;
    QString valueFilter_;
};