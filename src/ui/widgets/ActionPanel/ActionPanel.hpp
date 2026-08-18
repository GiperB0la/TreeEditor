#pragma once
#include <QWidget>

class QPushButton;

class ActionPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ActionPanel(QWidget* parent = nullptr);

signals:
    void addNodeClicked();
    void deleteNodeClicked();
    void addLeafClicked();
    void deleteLeafClicked();
    void editClicked();

private:
    QPushButton *addNodeButton_ = nullptr;
    QPushButton *deleteNodeButton_ = nullptr;
    QPushButton *addLeafButton_ = nullptr;
    QPushButton *deleteLeafButton_ = nullptr;
    QPushButton *editButton_ = nullptr;
};