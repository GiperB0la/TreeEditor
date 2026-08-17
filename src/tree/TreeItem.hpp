#pragma once
#include <QString>
#include <QVector>

struct LeafData
{
    int id = -1;
    int nodeId = -1;
    QString name;
    double value = 0.0;
};

struct NodeData
{
    int id = -1;
    QString name;
    QVector<LeafData> leaves;
};