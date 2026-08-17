#pragma once
#include <functional>

#include <QVector>
#include <QString>

#include "TreeItem.hpp"
#include "../database/TreeRepository.hpp"

class TreeManager
{
public:
    explicit TreeManager(TreeRepository &repository);

public:
    bool load();
    const QVector<NodeData> &nodes() const;
    QString lastError() const;

    bool addNode(const QString &name);
    bool deleteNode(int nodeId);
    bool addLeaf(
        int nodeId,
        const QString &name,
        double value);
    bool deleteLeaf(int leafId);
    bool updateLeaf(
        int leafId,
        const QString &name,
        double value);

    QVector<NodeData> filter(const QString& nameText, const QString& valueText) const;

    int nodeCount() const;
    int leafCount() const;
    double minimumValue() const;
    double maximumValue() const;

    bool exportJson(const QString &filePath) const;
    bool importJson(const QString &filePath);

private:
    QVector<NodeData> filter(const std::function<bool(const LeafData&)> &predicate) const;

private:
    TreeRepository &repository_;
    QVector<NodeData> nodes_;
    mutable QString lastError_;
};