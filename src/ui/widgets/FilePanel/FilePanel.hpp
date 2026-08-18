#pragma once
#include <QWidget>

class QPushButton;

class FilePanel : public QWidget
{
    Q_OBJECT

public:
    explicit FilePanel(QWidget *parent = nullptr);

signals:
    void exportClicked();
    void importClicked();
};