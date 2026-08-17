#pragma once
#include <QWidget>

class QLineEdit;
class QPushButton;
class QString;

class ConnectionPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionPanel(QWidget *parent = nullptr);

public:
    QString host() const;
    int port() const;
    QString database() const;
    QString user() const;
    QString password() const;

signals:
    void connectClicked();

private:
    QLineEdit *hostEdit_ = nullptr;
    QLineEdit *portEdit_ = nullptr;
    QLineEdit *databaseEdit_ = nullptr;
    QLineEdit *userEdit_ = nullptr;
    QLineEdit *passwordEdit_ = nullptr;
    QPushButton *connectButton_ = nullptr;
};