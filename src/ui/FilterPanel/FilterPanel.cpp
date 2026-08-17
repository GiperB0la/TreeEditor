#include "FilterPanel.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDoubleValidator>
#include <QLocale>

FilterPanel::FilterPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);

    auto *nameLabel = new QLabel("Фильтр по названию:", this);
    nameEdit_ = new QLineEdit(this);

    auto *valueLabel = new QLabel("Фильтр по значению:", this);
    valueEdit_ = new QLineEdit(this);

    auto* validator = new QDoubleValidator(valueEdit_);
    validator->setLocale(QLocale::c());
    valueEdit_->setValidator(validator);

    resetButton_ = new QPushButton("Сбросить фильтры", this);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit_);

    layout->addWidget(valueLabel);
    layout->addWidget(valueEdit_);

    layout->addWidget(resetButton_);

    connect(
        nameEdit_,
        &QLineEdit::textChanged,
        this,
        &FilterPanel::nameFilterChanged);

    connect(
        valueEdit_,
        &QLineEdit::textChanged,
        this,
        &FilterPanel::valueFilterChanged);

    connect(
        resetButton_,
        &QPushButton::clicked,
        this,
        &FilterPanel::resetClicked);
}

QString FilterPanel::nameFilter() const
{
    return nameEdit_->text();
}

QString FilterPanel::valueFilter() const
{
    return valueEdit_->text();
}

void FilterPanel::reset()
{
    nameEdit_->clear();
    valueEdit_->clear();
}