#include "FilePanel.hpp"

#include <QHBoxLayout>
#include <QPushButton>

FilePanel::FilePanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);

    auto *exportButton = new QPushButton("Выгрузить дерево", this);
    auto *importButton = new QPushButton("Загрузить дерево", this);

    layout->addWidget(exportButton);
    layout->addWidget(importButton);
    layout->addStretch();

    connect(exportButton, &QPushButton::clicked, this, &FilePanel::exportClicked);
    connect(importButton, &QPushButton::clicked, this, &FilePanel::importClicked);
}