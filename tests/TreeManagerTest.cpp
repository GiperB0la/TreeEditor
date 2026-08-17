#include <QtTest>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "../src/tree/TreeManager.hpp"

class TreeManagerTest : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase database_;
    TreeRepository* repository_ = nullptr;
    TreeManager* manager_ = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void filterByName();
    void filterByValue();
    void filterByNameAndValue();
    void filterWithoutFilters();

    void nodeCount();
    void leafCount();
    void minimumValue();
    void maximumValue();

    void addAndDeleteLeaf();
    void updateLeaf();
    void deleteNodeWithLeaves();
};

void TreeManagerTest::initTestCase()
{
    database_ = QSqlDatabase::addDatabase("QPSQL", "TreeManagerTest");

    database_.setHostName(qEnvironmentVariable("TEST_DB_HOST", "localhost"));
    database_.setPort(qEnvironmentVariableIntValue("TEST_DB_PORT"));
    database_.setDatabaseName(qEnvironmentVariable("TEST_DB_NAME", "treeeditor_test"));
    database_.setUserName(qEnvironmentVariable("TEST_DB_USER", "postgres"));
    database_.setPassword(qEnvironmentVariable("TEST_DB_PASSWORD", "postgres"));

    if (database_.port() == -1)
    {
        database_.setPort(5432);
    }

    QVERIFY2(
        database_.open(),
        qPrintable(database_.lastError().text()));

    repository_ = new TreeRepository(database_);
    manager_ = new TreeManager(*repository_);
}

void TreeManagerTest::cleanupTestCase()
{
    delete manager_;
    delete repository_;

    database_.close();
    QSqlDatabase::removeDatabase("TreeManagerTest");
}

void TreeManagerTest::init()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec("DELETE FROM leaves"));
    QVERIFY(query.exec("DELETE FROM nodes"));

    QVERIFY(query.exec(
        "ALTER SEQUENCE nodes_id_seq RESTART WITH 1"));

    QVERIFY(query.exec(
        "ALTER SEQUENCE leaves_id_seq RESTART WITH 1"));

    QVERIFY(manager_->load());
}

void TreeManagerTest::cleanup()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec("DELETE FROM leaves"));
    QVERIFY(query.exec("DELETE FROM nodes"));
}

void TreeManagerTest::filterByName()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'Apple', 10.0), "
        "       (1, 'Banana', 20.0), "
        "       (1, 'Pineapple', 30.0)"));

    QVERIFY(manager_->load());

    const QVector<NodeData> result =
        manager_->filter("apple", "");

    QCOMPARE(result.size(), 1);
    QCOMPARE(result[0].leaves.size(), 2);

    QCOMPARE(result[0].leaves[0].name, QString("Apple"));
    QCOMPARE(result[0].leaves[1].name, QString("Pineapple"));
}

void TreeManagerTest::filterByValue()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'First', 10.5), "
        "       (1, 'Second', 25.0), "
        "       (1, 'Third', 125.5)"));

    QVERIFY(manager_->load());

    const QVector<NodeData> result =
        manager_->filter("", "25");

    QCOMPARE(result.size(), 1);
    QCOMPARE(result[0].leaves.size(), 2);

    QCOMPARE(result[0].leaves[0].name, QString("Second"));
    QCOMPARE(result[0].leaves[1].name, QString("Third"));
}

void TreeManagerTest::filterByNameAndValue()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'Apple', 10.0), "
        "       (1, 'Apple Red', 20.0), "
        "       (1, 'Banana', 10.0)"));

    QVERIFY(manager_->load());

    const QVector<NodeData> result =
        manager_->filter("apple", "10");

    QCOMPARE(result.size(), 1);
    QCOMPARE(result[0].leaves.size(), 1);

    QCOMPARE(result[0].leaves[0].name, QString("Apple"));
    QCOMPARE(result[0].leaves[0].value, 10.0);
}

void TreeManagerTest::filterWithoutFilters()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node 1'), ('Node 2')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'Apple', 10.0), "
        "       (1, 'Banana', 20.0), "
        "       (2, 'Orange', 30.0)"));

    QVERIFY(manager_->load());

    const QVector<NodeData> result =
        manager_->filter("", "");

    QCOMPARE(result.size(), 2);
    QCOMPARE(result[0].leaves.size(), 2);
    QCOMPARE(result[1].leaves.size(), 1);
}

void TreeManagerTest::nodeCount()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) "
        "VALUES ('Node 1'), ('Node 2'), ('Node 3')"));

    QVERIFY(manager_->load());

    QCOMPARE(manager_->nodeCount(), 3);
}

void TreeManagerTest::leafCount()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) "
        "VALUES ('Node 1'), ('Node 2')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'A', 10.0), "
        "       (1, 'B', 20.0), "
        "       (2, 'C', 30.0)"));

    QVERIFY(manager_->load());

    QCOMPARE(manager_->leafCount(), 3);
}

void TreeManagerTest::minimumValue()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'A', 10.0), "
        "       (1, 'B', -5.5), "
        "       (1, 'C', 20.0), "
        "       (1, 'D', 3.14)"));

    QVERIFY(manager_->load());

    QCOMPARE(manager_->minimumValue(), -5.5);
}

void TreeManagerTest::maximumValue()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'A', 10.0), "
        "       (1, 'B', -5.5), "
        "       (1, 'C', 20.0), "
        "       (1, 'D', 3.14)"));

    QVERIFY(manager_->load());

    QCOMPARE(manager_->maximumValue(), 20.0);
}

void TreeManagerTest::addAndDeleteLeaf()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node')"));

    QVERIFY(manager_->load());

    QVERIFY(manager_->addLeaf(
        1,
        "Test leaf",
        42.5));

    QCOMPARE(manager_->leafCount(), 1);
    QCOMPARE(manager_->nodes()[0].leaves[0].name, QString("Test leaf"));
    QCOMPARE(manager_->nodes()[0].leaves[0].value, 42.5);

    const int leafId = manager_->nodes()[0].leaves[0].id;

    QVERIFY(manager_->deleteLeaf(leafId));

    QCOMPARE(manager_->leafCount(), 0);
}

void TreeManagerTest::updateLeaf()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) VALUES ('Node')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'Old name', 10.0)"));

    QVERIFY(manager_->load());

    const int leafId = manager_->nodes()[0].leaves[0].id;

    QVERIFY(manager_->updateLeaf(
        leafId,
        "New name",
        99.5));

    QCOMPARE(manager_->nodes()[0].leaves[0].name, QString("New name"));
    QCOMPARE(manager_->nodes()[0].leaves[0].value, 99.5);

    QSqlQuery checkQuery(database_);

    QVERIFY(checkQuery.exec(
        "SELECT name, value "
        "FROM leaves "
        "WHERE id = 1"));

    QVERIFY(checkQuery.next());

    QCOMPARE(
        checkQuery.value("name").toString(),
        QString("New name"));

    QCOMPARE(
        checkQuery.value("value").toDouble(),
        99.5);
}

void TreeManagerTest::deleteNodeWithLeaves()
{
    QSqlQuery query(database_);

    QVERIFY(query.exec(
        "INSERT INTO nodes (name) "
        "VALUES ('Node 1'), ('Node 2')"));

    QVERIFY(query.exec(
        "INSERT INTO leaves (node_id, name, value) "
        "VALUES (1, 'A', 10.0), "
        "       (1, 'B', 20.0), "
        "       (2, 'C', 30.0)"));

    QVERIFY(manager_->load());

    QCOMPARE(manager_->nodeCount(), 2);
    QCOMPARE(manager_->leafCount(), 3);

    QVERIFY(manager_->deleteNode(1));

    QCOMPARE(manager_->nodeCount(), 1);
    QCOMPARE(manager_->leafCount(), 1);

    QCOMPARE(manager_->nodes()[0].id, 2);
    QCOMPARE(manager_->nodes()[0].leaves[0].name, QString("C"));

    QSqlQuery checkQuery(database_);

    QVERIFY(checkQuery.exec(
        "SELECT COUNT(*) "
        "FROM leaves "
        "WHERE node_id = 1"));

    QVERIFY(checkQuery.next());

    QCOMPARE(checkQuery.value(0).toInt(), 0);
}

QTEST_MAIN(TreeManagerTest)

#include "TreeManagerTest.moc"