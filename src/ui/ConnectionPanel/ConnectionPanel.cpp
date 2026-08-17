#include "ConnectionPanel.hpp"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

ConnectionPanel::ConnectionPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);

    auto *hostLabel = new QLabel("Host:", this);
    hostEdit_ = new QLineEdit("127.0.0.1", this);

    auto *portLabel = new QLabel("Порт:", this);
    portEdit_ = new QLineEdit("5432", this);
    portEdit_->setValidator(new QIntValidator(1, 65535, portEdit_));

    auto *databaseLabel = new QLabel("БД:", this);
    databaseEdit_ = new QLineEdit("tree_editor", this);

    auto *userLabel = new QLabel("Пользователь:", this);
    userEdit_ = new QLineEdit("postgres", this);

    auto *passwordLabel = new QLabel("Пароль:", this);
    passwordEdit_ = new QLineEdit(this);
    passwordEdit_->setEchoMode(QLineEdit::Password);

    connectButton_ = new QPushButton("Подключиться", this);

    layout->addWidget(hostLabel);
    layout->addWidget(hostEdit_);

    layout->addWidget(portLabel);
    layout->addWidget(portEdit_);

    layout->addWidget(databaseLabel);
    layout->addWidget(databaseEdit_);

    layout->addWidget(userLabel);
    layout->addWidget(userEdit_);

    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit_);

    layout->addWidget(connectButton_);

    connect(
        connectButton_,
        &QPushButton::clicked,
        this,
        &ConnectionPanel::connectClicked);
}

QString ConnectionPanel::host() const
{
    return hostEdit_->text();
}

int ConnectionPanel::port() const
{
    return portEdit_->text().toInt();
}

QString ConnectionPanel::database() const
{
    return databaseEdit_->text();
}

QString ConnectionPanel::user() const
{
    return userEdit_->text();
}

QString ConnectionPanel::password() const
{
    return passwordEdit_->text();
}