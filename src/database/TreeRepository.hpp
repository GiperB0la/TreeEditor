#pragma once
#include <QSqlDatabase>
#include <QVector>
#include <QString>

#include "../Tree/TreeItem.hpp"

class TreeRepository
{
public:
    explicit TreeRepository(const QSqlDatabase& database);

public:
    bool loadTree(QVector<NodeData> &nodes) const;
    bool replaceTree(const QVector<NodeData> &nodes);
    QString lastError() const;

    bool addNode(const QString& name, int &nodeId);
    bool deleteNode(int nodeId);
    bool addLeaf(
        int nodeId,
        const QString &name,
        double value,
        int &leafId);
    bool deleteLeaf(int leafId);
    bool updateLeaf(
        int leafId,
        const QString &name,
        double value);

private:
    QSqlDatabase database_;
    mutable QString lastError_;
};