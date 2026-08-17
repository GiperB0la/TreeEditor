#pragma once
#include <QSqlDatabase>

class QString;

class DatabaseManager
{
public:
    ~DatabaseManager();

public:
    bool connect(
        const QString &host, 
        int port, 
        const QString &database, 
        const QString &user, 
        const QString &password);

    void disconnect();
    bool isConnected() const;

    QSqlDatabase &database();
    QString lastError() const;

private:
    QSqlDatabase database_{
        QSqlDatabase::addDatabase("QPSQL", "tree_connection")
    };
};