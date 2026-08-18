#include "ActionPanel.hpp"

#include <QPushButton>
#include <QHBoxLayout>

ActionPanel::ActionPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);

    addNodeButton_ = new QPushButton("Добавить узел", this);
    deleteNodeButton_ = new QPushButton("Удалить узел", this);
    addLeafButton_ = new QPushButton("Добавить лист", this);
    deleteLeafButton_ = new QPushButton("Удалить лист", this);
    editButton_ = new QPushButton("Редактировать", this);

    layout->addWidget(addNodeButton_);
    layout->addWidget(deleteNodeButton_);
    layout->addWidget(addLeafButton_);
    layout->addWidget(deleteLeafButton_);
    layout->addWidget(editButton_);

    connect(
        addNodeButton_, 
        &QPushButton::clicked,
        this, 
        &ActionPanel::addNodeClicked);

    connect(
        deleteNodeButton_, 
        &QPushButton::clicked,
        this, 
        &ActionPanel::deleteNodeClicked);

    connect(
        addLeafButton_, 
        &QPushButton::clicked,
        this, 
        &ActionPanel::addLeafClicked);

    connect(
        deleteLeafButton_, 
        &QPushButton::clicked,
        this, 
        &ActionPanel::deleteLeafClicked);

    connect(
        editButton_, 
        &QPushButton::clicked,
        this, 
        &ActionPanel::editClicked);
}