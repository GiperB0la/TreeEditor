#include "StatisticsPanel.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QString>

StatisticsPanel::StatisticsPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(20);

    nodeCountLabel_ = new QLabel("Узлов: 0", this);
    leafCountLabel_ = new QLabel("Листов: 0", this);
    minimumLabel_ = new QLabel("Минимум: -", this);
    maximumLabel_ = new QLabel("Максимум: -", this);

    layout->addWidget(nodeCountLabel_);
    layout->addWidget(leafCountLabel_);
    layout->addStretch();
    layout->addWidget(minimumLabel_);
    layout->addWidget(maximumLabel_);

    nodeCountLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    leafCountLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    minimumLabel_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    maximumLabel_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}

void StatisticsPanel::setNodeCount(int count)
{
    nodeCountLabel_->setText(QString("Узлов: %1").arg(count));
}

void StatisticsPanel::setLeafCount(int count)
{
    leafCountLabel_->setText(QString("Листов: %1").arg(count));
}

void StatisticsPanel::setMinimum(double value)
{
    minimumLabel_->setText(QString("Минимум: %1").arg(value));
}

void StatisticsPanel::setMaximum(double value)
{
    maximumLabel_->setText(QString("Максимум: %1").arg(value));
}

void StatisticsPanel::reset(int nodeCount)
{
    nodeCountLabel_->setText(QString("Узлов: %1").arg(nodeCount));
    leafCountLabel_->setText("Листов: 0");
    minimumLabel_->setText("Минимум: -");
    maximumLabel_->setText("Максимум: -");
}