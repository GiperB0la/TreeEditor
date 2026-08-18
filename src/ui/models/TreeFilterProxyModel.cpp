#include "TreeFilterProxyModel.hpp"

#include "../models/TreeModel.hpp"

TreeFilterProxyModel::TreeFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void TreeFilterProxyModel::setNameFilter(const QString &text)
{
    nameFilter_ = text;
    beginFilterChange();
    endFilterChange();
}

void TreeFilterProxyModel::setValueFilter(const QString &text)
{
    valueFilter_ = text;
    beginFilterChange();
    endFilterChange();
}

bool TreeFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(
        sourceRow,
        0,
        sourceParent);

    if (!index.isValid())
    {
        return false;
    }

    if (!sourceParent.isValid())
    {
        const int leafCount = sourceModel()->rowCount(index);

        for (int leafRow = 0; leafRow < leafCount; ++leafRow)
        {
            if (filterAcceptsRow(leafRow, index))
            {
                return true;
            }
        }

        return false;
    }

    const QString name = sourceModel()->data(index, Qt::DisplayRole).toString();

    const QModelIndex valueIndex = sourceModel()->index(
        sourceRow,
        1,
        sourceParent);

    const double value = sourceModel()->data(valueIndex, Qt::DisplayRole).toDouble();

    const bool nameMatches =
        nameFilter_.isEmpty() ||
        name.contains(
            nameFilter_,
            Qt::CaseInsensitive);

    const QString valueText = QString::number(value, 'g', 15);

    const bool valueMatches = valueFilter_.isEmpty() || valueText.contains(valueFilter_);

    return nameMatches && valueMatches;
}