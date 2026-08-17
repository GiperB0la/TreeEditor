#include "TreeRepository.hpp"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

TreeRepository::TreeRepository(const QSqlDatabase &database)
    : database_(database)
{
}

bool TreeRepository::loadTree(QVector<NodeData> &nodes) const
{
    nodes.clear();

    QSqlQuery nodeQuery(database_);

    if (!nodeQuery.exec(
        "SELECT id, name "
        "FROM nodes "
        "ORDER BY id"))
    {
        lastError_ = nodeQuery.lastError().text();
        qDebug() << "Failed to load nodes:" << lastError_;
        return false;
    }

    while (nodeQuery.next())
    {
        NodeData node;

        node.id = nodeQuery.value("id").toInt();
        node.name = nodeQuery.value("name").toString();

        QSqlQuery leafQuery(database_);

        leafQuery.prepare(
            "SELECT id, node_id, name, value "
            "FROM leaves "
            "WHERE node_id = :node_id "
            "ORDER BY id");

        leafQuery.bindValue(":node_id", node.id);

        if (!leafQuery.exec())
        {
            lastError_ = leafQuery.lastError().text();
            qDebug() << "Failed to load leaves for node"
                << node.id << ":" << lastError_;
            return false;
        }

        while (leafQuery.next())
        {
            LeafData leaf;

            leaf.id = leafQuery.value("id").toInt();
            leaf.nodeId = leafQuery.value("node_id").toInt();
            leaf.name = leafQuery.value("name").toString();
            leaf.value = leafQuery.value("value").toDouble();

            node.leaves.append(leaf);
        }

        nodes.append(node);
    }

    lastError_.clear();

    return true;
}

bool TreeRepository::replaceTree(const QVector<NodeData> &nodes)
{
    if (!database_.transaction())
    {
        lastError_ = database_.lastError().text();
        qDebug() << "Failed to start transaction:" << lastError_;
        return false;
    }

    QSqlQuery query(database_);

    if (!query.exec("DELETE FROM leaves"))
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to delete leaves:" << lastError_;

        database_.rollback();
        return false;
    }

    if (!query.exec("DELETE FROM nodes"))
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to delete nodes:" << lastError_;

        database_.rollback();
        return false;
    }

    query.prepare(
        "INSERT INTO nodes (id, name) "
        "VALUES (:id, :name)");

    for (const NodeData &node : nodes)
    {
        query.bindValue(":id", node.id);
        query.bindValue(":name", node.name);

        if (!query.exec())
        {
            lastError_ = query.lastError().text();
            qDebug() << "Failed to insert node:" << lastError_;

            database_.rollback();
            return false;
        }
    }

    query.prepare(
        "INSERT INTO leaves (id, node_id, name, value) "
        "VALUES (:id, :node_id, :name, :value)");

    for (const NodeData &node : nodes)
    {
        for (const LeafData &leaf : node.leaves)
        {
            query.bindValue(":id", leaf.id);
            query.bindValue(":node_id", leaf.nodeId);
            query.bindValue(":name", leaf.name);
            query.bindValue(":value", leaf.value);

            if (!query.exec())
            {
                lastError_ = query.lastError().text();
                qDebug() << "Failed to insert leaf:" << lastError_;

                database_.rollback();
                return false;
            }
        }
    }

    if (!query.exec(
        "SELECT setval("
        "pg_get_serial_sequence('nodes', 'id'), "
        "COALESCE((SELECT MAX(id) FROM nodes), 1), "
        "(SELECT COUNT(*) > 0 FROM nodes))"))
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to reset nodes sequence:" << lastError_;

        database_.rollback();
        return false;
    }

    if (!query.exec(
        "SELECT setval("
        "pg_get_serial_sequence('leaves', 'id'), "
        "COALESCE((SELECT MAX(id) FROM leaves), 1), "
        "(SELECT COUNT(*) > 0 FROM leaves))"))
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to reset leaves sequence:" << lastError_;

        database_.rollback();
        return false;
    }

    if (!database_.commit())
    {
        lastError_ = database_.lastError().text();
        qDebug() << "Failed to commit transaction:" << lastError_;

        database_.rollback();
        return false;
    }

    lastError_.clear();

    return true;
}

QString TreeRepository::lastError() const
{
    return lastError_;
}

bool TreeRepository::addNode(const QString &name, int &nodeId)
{
    QSqlQuery query(database_);

    query.prepare(
        "INSERT INTO nodes (name) "
        "VALUES (:name) "
        "RETURNING id");

    query.bindValue(":name", name);

    if (!query.exec())
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to add node:" << lastError_;
        return false;
    }

    if (!query.next())
    {
        lastError_ = query.lastError().text();

        if (lastError_.isEmpty())
        {
            lastError_ = "Не удалось получить ID добавленного узла.";
        }

        qDebug() << "Failed to get inserted node id:" << lastError_;
        return false;
    }

    nodeId = query.value("id").toInt();

    lastError_.clear();

    return true;
}

bool TreeRepository::deleteNode(int nodeId)
{
    QSqlQuery query(database_);

    query.prepare(
        "DELETE FROM nodes "
        "WHERE id = :id");

    query.bindValue(":id", nodeId);

    if (!query.exec())
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to delete node:" << lastError_;
        return false;
    }

    if (query.numRowsAffected() != 1)
    {
        lastError_ = "Узел не найден.";
        return false;
    }

    lastError_.clear();

    return true;
}

bool TreeRepository::addLeaf(
    int nodeId,
    const QString &name,
    double value,
    int &leafId)
{
    QSqlQuery query(database_);

    query.prepare(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (:node_id, :name, :value) "
        "RETURNING id");

    query.bindValue(":node_id", nodeId);
    query.bindValue(":name", name);
    query.bindValue(":value", value);

    if (!query.exec())
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to add leaf:" << lastError_;
        return false;
    }

    if (!query.next())
    {
        lastError_ = query.lastError().text();

        if (lastError_.isEmpty())
        {
            lastError_ = "Не удалось получить ID добавленного листа.";
        }

        qDebug() << "Failed to get inserted leaf id:" << lastError_;
        return false;
    }

    leafId = query.value("id").toInt();

    lastError_.clear();

    return true;
}

bool TreeRepository::deleteLeaf(int leafId)
{
    QSqlQuery query(database_);

    query.prepare(
        "DELETE FROM leaves "
        "WHERE id = :id");

    query.bindValue(":id", leafId);

    if (!query.exec())
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to delete leaf:" << lastError_;
        return false;
    }

    if (query.numRowsAffected() != 1)
    {
        lastError_ = "Лист не найден.";
        return false;
    }

    lastError_.clear();

    return true;
}

bool TreeRepository::updateLeaf(
    int leafId,
    const QString &name,
    double value)
{
    QSqlQuery query(database_);

    query.prepare(
        "UPDATE leaves "
        "SET name = :name, "
        "    value = :value "
        "WHERE id = :id");

    query.bindValue(":id", leafId);
    query.bindValue(":name", name);
    query.bindValue(":value", value);

    if (!query.exec())
    {
        lastError_ = query.lastError().text();
        qDebug() << "Failed to update leaf:" << lastError_;
        return false;
    }

    if (query.numRowsAffected() != 1)
    {
        lastError_ = "Лист не найден.";
        return false;
    }

    lastError_.clear();

    return true;
}