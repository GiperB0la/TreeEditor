#pragma once
#include <QObject>
#include <QVector>
#include <QString>

#include "TreeItem.hpp"
#include "../database/TreeRepository.hpp"

class TreeManager : public QObject
{
    Q_OBJECT

public:
    explicit TreeManager(
        TreeRepository &repository,
        QObject *parent = nullptr);

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

    int nodeCount() const;
    int leafCount() const;
    double minimumValue() const;
    double maximumValue() const;

    bool exportJson(const QString &filePath) const;
    bool importJson(const QString &filePath);

signals:
    void nodeAboutToBeAdded(int row);
    void nodeAdded();

    void nodeAboutToBeRemoved(int row);
    void nodeRemoved();

    void leafAboutToBeAdded(
        int nodeRow,
        int leafRow);

    void leafAdded();

    void leafAboutToBeRemoved(
        int nodeRow,
        int leafRow);

    void leafRemoved();

    void leafChanged(
        int nodeRow,
        int leafRow);

private:
    TreeRepository &repository_;
    QVector<NodeData> nodes_;
    mutable QString lastError_;
};