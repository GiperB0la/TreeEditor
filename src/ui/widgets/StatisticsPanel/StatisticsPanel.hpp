#pragma once
#include <QWidget>

class QLabel;

class StatisticsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPanel(QWidget *parent = nullptr);

    void setNodeCount(int count);
    void setLeafCount(int count);
    void setMinimum(double value);
    void setMaximum(double value);

    void reset(int nodeCount);

private:
    QLabel *nodeCountLabel_;
    QLabel *leafCountLabel_;
    QLabel *minimumLabel_;
    QLabel *maximumLabel_;
};