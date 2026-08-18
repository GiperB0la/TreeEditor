#include <QtTest>

#include "../src/tree/TreeManager.hpp"
#include "../src/database/TreeRepository.hpp"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTemporaryDir>

class TreeManagerTest : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase database_;
    QString connectionName_;

    void createTables()
    {
        QSqlQuery query(database_);

        QVERIFY(query.exec(
            "CREATE TABLE nodes ("
            "id SERIAL PRIMARY KEY,"
            "name TEXT NOT NULL"
            ")"));

        QVERIFY(query.exec(
            "CREATE TABLE leaves ("
            "id SERIAL PRIMARY KEY,"
            "node_id INTEGER REFERENCES nodes(id) ON DELETE CASCADE,"
            "name TEXT NOT NULL,"
            "value DOUBLE PRECISION"
            ")"));
    }

private slots:
    void initTestCase()
    {
        database_ = QSqlDatabase::addDatabase("QPSQL", "test_connection");
        database_.setHostName(qEnvironmentVariable("TEST_DB_HOST"));
        database_.setPort(qEnvironmentVariableIntValue("TEST_DB_PORT"));
        database_.setDatabaseName(qEnvironmentVariable("TEST_DB_NAME"));
        database_.setUserName(qEnvironmentVariable("TEST_DB_USER"));
        database_.setPassword(qEnvironmentVariable("TEST_DB_PASSWORD"));

        if (!database_.open())
        {
            qFatal(
                "Database connection failed: %s",
                qPrintable(
                    database_.lastError().text()));
        }

        createTables();
    }

    void cleanupTestCase()
    {
        database_.close();
        QSqlDatabase::removeDatabase(connectionName_);
    }

    void init()
    {
        QSqlQuery query(database_);
        query.exec("TRUNCATE leaves, nodes RESTART IDENTITY CASCADE");
    }

    void testAddNode()
    {
        TreeRepository repository(database_);
        TreeManager manager(repository);

        QSignalSpy aboutSpy(
            &manager,
            &TreeManager::nodeAboutToBeAdded);

        QSignalSpy addedSpy(
            &manager,
            &TreeManager::nodeAdded);

        QVERIFY(manager.load());

        QVERIFY(manager.addNode("Engine"));

        QCOMPARE(manager.nodeCount(), 1);

        QCOMPARE(
            aboutSpy.count(),
            1);

        QCOMPARE(
            addedSpy.count(),
            1);

        QCOMPARE(
            manager.nodes()[0].name,
            QString("Engine"));
    }

    void testAddLeaf()
    {
        TreeRepository repository(database_);
        TreeManager manager(repository);

        QVERIFY(manager.load());

        QVERIFY(manager.addNode("Motor"));

        int nodeId = manager.nodes()[0].id;

        QSignalSpy spy(
            &manager,
            &TreeManager::leafAdded);

        QVERIFY(
            manager.addLeaf(
                nodeId,
                "Temperature",
                85.5));

        QCOMPARE(
            manager.leafCount(),
            1);

        QCOMPARE(
            spy.count(),
            1);

        const LeafData &leaf = manager.nodes()[0].leaves[0];

        QCOMPARE(
            leaf.name,
            QString("Temperature"));

        QCOMPARE(
            leaf.value,
            85.5);
    }

    void testUpdateLeaf()
    {
        TreeRepository repository(database_);
        TreeManager manager(repository);

        QVERIFY(manager.load());

        QVERIFY(
            manager.addNode("Node"));

        int nodeId = manager.nodes()[0].id;

        QVERIFY(
            manager.addLeaf(
                nodeId,
                "Value",
                10));

        int leafId =
            manager.nodes()[0]
                .leaves[0]
                .id;

        QSignalSpy spy(
            &manager,
            &TreeManager::leafChanged);

        QVERIFY(
            manager.updateLeaf(
                leafId,
                "NewValue",
                20));

        QCOMPARE(
            spy.count(),
            1);

        QCOMPARE(
            manager.nodes()[0]
                .leaves[0]
                .name,
            QString("NewValue"));

        QCOMPARE(
            manager.nodes()[0]
                .leaves[0]
                .value,
            20.0);
    }

    void testDeleteLeaf()
    {
        TreeRepository repository(database_);
        TreeManager manager(repository);

        QVERIFY(manager.load());

        QVERIFY(
            manager.addNode("Node"));

        int nodeId = manager.nodes()[0].id;

        QVERIFY(
            manager.addLeaf(
                nodeId,
                "Leaf",
                5));

        int leafId =
            manager.nodes()[0]
                .leaves[0]
                .id;

        QSignalSpy spy(
            &manager,
            &TreeManager::leafRemoved);

        QVERIFY(
            manager.deleteLeaf(
                leafId));

        QCOMPARE(
            manager.leafCount(),
            0);

        QCOMPARE(
            spy.count(),
            1);
    }

    void testDeleteNode()
    {
        TreeRepository repository(database_);
        TreeManager manager(repository);

        QVERIFY(manager.load());

        QVERIFY(
            manager.addNode("Node"));

        int id = manager.nodes()[0].id;

        QSignalSpy spy(
            &manager,
            &TreeManager::nodeRemoved);

        QVERIFY(
            manager.deleteNode(id));

        QCOMPARE(
            manager.nodeCount(),
            0);

        QCOMPARE(
            spy.count(),
            1);
    }

    void testStatistics()
    {
        TreeRepository repository(database_);
        TreeManager manager(repository);

        QVERIFY(manager.load());

        manager.addNode("A");
        manager.addNode("B");

        QVERIFY(
            manager.addLeaf(
                manager.nodes()[0].id,
                "L1",
                50));

        QVERIFY(
            manager.addLeaf(
                manager.nodes()[1].id,
                "L2",
                10));

        QVERIFY(
            manager.addLeaf(
                manager.nodes()[1].id,
                "L3",
                100));

        QCOMPARE(
            manager.nodeCount(),
            2);

        QCOMPARE(
            manager.leafCount(),
            3);

        QCOMPARE(
            manager.minimumValue(),
            10.0);

        QCOMPARE(
            manager.maximumValue(),
            100.0);
    }

    void testJsonExportImport()
    {
        TreeRepository repository(database_);
        TreeManager manager(repository);

        QVERIFY(manager.load());

        manager.addNode("Node");

        QVERIFY(
            manager.addLeaf(
                manager.nodes()[0].id,
                "Value",
                42));

        QTemporaryDir dir;

        QString file = dir.filePath("tree.json");

        QVERIFY(
            manager.exportJson(file));

        QVERIFY(
            manager.importJson(file));

        QCOMPARE(
            manager.nodeCount(),
            1);

        QCOMPARE(
            manager.leafCount(),
            1);

        QCOMPARE(
            manager.nodes()[0]
                .leaves[0]
                .value,
            42.0);
    }
};

QTEST_MAIN(TreeManagerTest)

#include "TreeManagerTest.moc"