#pragma once
#include <QWidget>

class QLineEdit;
class QPushButton;
class QString;

class FilterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FilterPanel(QWidget* parent = nullptr);

public:
    QString nameFilter() const;
    QString valueFilter() const;
    void reset();

signals:
    void nameFilterChanged(const QString& text);
    void valueFilterChanged(const QString& text);
    void resetClicked();

private:
    QLineEdit* nameEdit_ = nullptr;
    QLineEdit* valueEdit_ = nullptr;
    QPushButton* resetButton_ = nullptr;
};