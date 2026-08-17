#include "DatabaseManager.hpp"

#include <QDebug>
#include <QString>
#include <QSqlError>

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

bool DatabaseManager::connect(
    const QString &host, 
    int port, 
    const QString &database, 
    const QString &user, 
    const QString &password)
{
    if (!QSqlDatabase::isDriverAvailable("QPSQL")) 
    {
        qDebug() << "QPSQL driver is not available";
        return false;
    }

    disconnect();

    database_.setHostName(host);
    database_.setPort(port);
    database_.setDatabaseName(database);
    database_.setUserName(user);
    database_.setPassword(password);

    if (!database_.open()) 
    {
        return false;
    }

    qDebug() << "Database connected successfully";

    return true;
}

void DatabaseManager::disconnect()
{
    if (database_.isOpen()) 
    {
        database_.close();
        qDebug() << "Database disconnected";
    }
}

bool DatabaseManager::isConnected() const
{
    return database_.isOpen();
}

QSqlDatabase &DatabaseManager::database()
{
    return database_;
}

QString DatabaseManager::lastError() const
{
    return database_.lastError().text();
}