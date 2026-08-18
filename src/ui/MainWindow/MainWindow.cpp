#include "MainWindow.hpp"

#include <QVBoxLayout>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QTreeView>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), treeModel_(treeManager_.nodes())
{
    setupUi();
    setupConnections();
}

void MainWindow::setupUi()
{
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);

    connectionPanel_ = new ConnectionPanel(centralWidget);
    treePanel_ = new TreePanel(&treeModel_, centralWidget);
    actionPanel_ = new ActionPanel(centralWidget);
    filterPanel_ = new FilterPanel(centralWidget);
    filePanel_ = new FilePanel(centralWidget);
    statisticsPanel_ = new StatisticsPanel(centralWidget);

    mainLayout->addWidget(connectionPanel_);
    mainLayout->addWidget(treePanel_, 1);
    mainLayout->addWidget(actionPanel_);
    mainLayout->addWidget(filterPanel_);
    mainLayout->addWidget(filePanel_);
    mainLayout->addWidget(statisticsPanel_);

    setCentralWidget(centralWidget);

    setWindowTitle("TreeEditor");
    resize(900, 650);
}

void MainWindow::setupConnections()
{
    connect(
        &treeManager_,
        &TreeManager::nodeAboutToBeAdded,
        &treeModel_,
        &TreeModel::nodeAboutToBeAdded);

    connect(
        &treeManager_,
        &TreeManager::nodeAdded,
        &treeModel_,
        &TreeModel::nodeAdded);

    connect(
        &treeManager_,
        &TreeManager::nodeAboutToBeRemoved,
        &treeModel_,
        &TreeModel::nodeAboutToBeRemoved);

    connect(
        &treeManager_,
        &TreeManager::nodeRemoved,
        &treeModel_,
        &TreeModel::nodeRemoved);

    connect(
        &treeManager_,
        &TreeManager::leafAboutToBeAdded,
        &treeModel_,
        &TreeModel::leafAboutToBeAdded);

    connect(
        &treeManager_,
        &TreeManager::leafAdded,
        &treeModel_,
        &TreeModel::leafAdded);

    connect(
        &treeManager_,
        &TreeManager::leafAboutToBeRemoved,
        &treeModel_,
        &TreeModel::leafAboutToBeRemoved);

    connect(
        &treeManager_,
        &TreeManager::leafRemoved,
        &treeModel_,
        &TreeModel::leafRemoved);

    connect(
        &treeManager_,
        &TreeManager::leafChanged,
        &treeModel_,
        &TreeModel::leafChanged);

    connect(
        connectionPanel_,
        &ConnectionPanel::connectClicked,
        this,
        &MainWindow::onConnectClicked);

    connect(
        actionPanel_,
        &ActionPanel::addNodeClicked,
        this,
        &MainWindow::onAddNodeClicked);

    connect(
        actionPanel_,
        &ActionPanel::deleteNodeClicked,
        this,
        &MainWindow::onDeleteNodeClicked);

    connect(
        actionPanel_,
        &ActionPanel::addLeafClicked,
        this,
        &MainWindow::onAddLeafClicked);

    connect(
        actionPanel_,
        &ActionPanel::deleteLeafClicked,
        this,
        &MainWindow::onDeleteLeafClicked);

    connect(
        actionPanel_,
        &ActionPanel::editClicked,
        this,
        &MainWindow::onEditClicked);

    connect(
        filterPanel_,
        &FilterPanel::nameFilterChanged,
        this,
        &MainWindow::onFilterChanged);

    connect(
        filterPanel_,
        &FilterPanel::valueFilterChanged,
        this,
        &MainWindow::onFilterChanged);

    connect(
        filterPanel_,
        &FilterPanel::resetClicked,
        this,
        &MainWindow::onResetFilters);

    connect(
        filePanel_,
        &FilePanel::exportClicked,
        this,
        &MainWindow::onExportClicked);

    connect(
        filePanel_,
        &FilePanel::importClicked,
        this,
        &MainWindow::onImportClicked);
}

void MainWindow::onConnectClicked()
{
    if (!databaseManager_.connect(
        connectionPanel_->host(),
        connectionPanel_->port(),
        connectionPanel_->database(),
        connectionPanel_->user(),
        connectionPanel_->password()))
    {
        QMessageBox::critical(
            this,
            "Ошибка подключения",
            "Не удалось подключиться к базе данных:\n\n" +
                databaseManager_.lastError());
        return;
    }

    if (!treeManager_.load())
    {
        QMessageBox::critical(
            this,
            "Ошибка загрузки",
            "Не удалось загрузить дерево:\n\n" +
                treeManager_.lastError());
        return;
    }

    treeModel_.reset();
    treePanel_->treeView()->expandAll();

    updateStatistics();
}

void MainWindow::onAddNodeClicked()
{
    if (!databaseManager_.isConnected())
    {
        QMessageBox::warning(
            this,
            "Нет соединения",
            "Сначала подключитесь к базе данных.");

        return;
    }

    bool ok = false;

    const QString name = QInputDialog::getText(
        this,
        "Добавить узел",
        "Название:",
        QLineEdit::Normal,
        {},
        &ok);

    if (!ok || name.trimmed().isEmpty())
    {
        return;
    }

    if (!treeManager_.addNode(name.trimmed()))
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            treeManager_.lastError());

        return;
    }

    updateStatistics();
}

void MainWindow::onDeleteNodeClicked()
{
    if (!databaseManager_.isConnected())
    {
        QMessageBox::warning(
            this,
            "Нет соединения",
            "Сначала подключитесь к базе данных.");

        return;
    }

    const QModelIndex index = treePanel_->currentIndex();

    if (!index.isValid() || index.parent().isValid())
    {
        QMessageBox::warning(
            this,
            "Удаление узла",
            "Выберите узел.");

        return;
    }

    const int row = index.row();

    const QVector<NodeData>& nodes = treeManager_.nodes();

    if (row < 0 || row >= nodes.size())
    {
        return;
    }

    const int nodeId = nodes[row].id;

    const auto result = QMessageBox::question(
        this,
        "Удаление узла",
        "Удалить выбранный узел и все его листья?");

    if (result != QMessageBox::Yes)
    {
        return;
    }

    if (!treeManager_.deleteNode(nodeId))
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            treeManager_.lastError());

        return;
    }

    updateStatistics();
}

void MainWindow::onAddLeafClicked()
{
    if (!databaseManager_.isConnected())
    {
        QMessageBox::warning(
            this,
            "Нет соединения",
            "Сначала подключитесь к базе данных.");

        return;
    }

    const QModelIndex index = treePanel_->currentIndex();

    if (!index.isValid())
    {
        QMessageBox::warning(
            this,
            "Добавление листа",
            "Выберите узел.");

        return;
    }

    const int nodeRow = index.parent().isValid()
        ? index.parent().row()
        : index.row();

    const QVector<NodeData> &nodes = treeManager_.nodes();

    if (nodeRow < 0 || nodeRow >= nodes.size())
    {
        return;
    }

    const int nodeId = nodes[nodeRow].id;

    bool ok = false;

    const QString name = QInputDialog::getText(
        this,
        "Добавить лист",
        "Название:",
        QLineEdit::Normal,
        {},
        &ok);

    if (!ok || name.trimmed().isEmpty())
    {
        return;
    }

    const double value = QInputDialog::getDouble(
        this,
        "Добавить лист",
        "Значение:",
        0.0,
        -1e100,
        1e100,
        6,
        &ok);

    if (!ok)
    {
        return;
    }

    if (!treeManager_.addLeaf(
        nodeId,
        name.trimmed(),
        value))
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            treeManager_.lastError());

        return;
    }

    updateStatistics();
}

void MainWindow::onDeleteLeafClicked()
{
    if (!databaseManager_.isConnected())
    {
        QMessageBox::warning(
            this,
            "Нет соединения",
            "Сначала подключитесь к базе данных.");

        return;
    }

    const QModelIndex index = treePanel_->currentIndex();

    if (!index.isValid() || !index.parent().isValid())
    {
        QMessageBox::warning(
            this,
            "Удаление листа",
            "Выберите лист.");

        return;
    }

    const int nodeRow = index.parent().row();
    const int leafRow = index.row();

    const QVector<NodeData>& nodes = treeManager_.nodes();

    if (nodeRow < 0 || nodeRow >= nodes.size())
    {
        return;
    }

    const NodeData& node = nodes[nodeRow];

    if (leafRow < 0 || leafRow >= node.leaves.size())
    {
        return;
    }

    const int leafId = node.leaves[leafRow].id;

    const auto result = QMessageBox::question(
        this,
        "Удаление листа",
        "Удалить выбранный лист?");

    if (result != QMessageBox::Yes)
    {
        return;
    }

    if (!treeManager_.deleteLeaf(leafId))
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            treeManager_.lastError());

        return;
    }

    updateStatistics();
}

void MainWindow::onEditClicked()
{
    if (!databaseManager_.isConnected())
    {
        QMessageBox::warning(
            this,
            "Нет соединения",
            "Сначала подключитесь к базе данных.");

        return;
    }

    const QModelIndex index = treePanel_->currentIndex();

    if (!index.isValid() || !index.parent().isValid())
    {
        QMessageBox::warning(
            this,
            "Редактирование",
            "Выберите лист.");

        return;
    }

    const int nodeRow = index.parent().row();
    const int leafRow = index.row();

    const QVector<NodeData>& nodes = treeManager_.nodes();

    if (nodeRow < 0 || nodeRow >= nodes.size())
    {
        return;
    }

    const NodeData& node = nodes[nodeRow];

    if (leafRow < 0 || leafRow >= node.leaves.size())
    {
        return;
    }

    const LeafData& leaf = node.leaves[leafRow];

    bool ok = false;

    const QString name = QInputDialog::getText(
        this,
        "Редактировать лист",
        "Название:",
        QLineEdit::Normal,
        leaf.name,
        &ok);

    if (!ok || name.trimmed().isEmpty())
    {
        return;
    }

    const double value = QInputDialog::getDouble(
        this,
        "Редактировать лист",
        "Значение:",
        leaf.value,
        -1e100,
        1e100,
        6,
        &ok);

    if (!ok)
    {
        return;
    }

    if (!treeManager_.updateLeaf(
        leaf.id,
        name.trimmed(),
        value))
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            treeManager_.lastError());

        return;
    }

    updateStatistics();
}

void MainWindow::onFilterChanged()
{
    treePanel_->setNameFilter(filterPanel_->nameFilter());
    treePanel_->setValueFilter(filterPanel_->valueFilter());
}

void MainWindow::onResetFilters()
{
    filterPanel_->reset();
}

void MainWindow::onExportClicked()
{
    if (!databaseManager_.isConnected())
    {
        QMessageBox::warning(this, "Нет соединения", "Сначала подключитесь к базе данных.");
        return;
    }

    const QString filePath = QFileDialog::getSaveFileName(
        this,
        "Выгрузить дерево",
        QString(),
        "JSON (*.json)");

    if (filePath.isEmpty())
    {
        return;
    }

    if (!treeManager_.exportJson(filePath))
    {
        QMessageBox::critical(
            this,
            "Ошибка экспорта",
            treeManager_.lastError());

        return;
    }
}

void MainWindow::onImportClicked()
{
    if (!databaseManager_.isConnected())
    {
        QMessageBox::warning(
            this,
            "Нет соединения",
            "Сначала подключитесь к базе данных.");

        return;
    }

    const QString filePath = QFileDialog::getOpenFileName(
        this,
        "Загрузить дерево",
        QString(),
        "JSON (*.json)");

    if (filePath.isEmpty())
    {
        return;
    }

    if (!treeManager_.importJson(filePath))
    {
        QMessageBox::critical(
            this,
            "Ошибка импорта",
            treeManager_.lastError());

        return;
    }

    treeModel_.reset();
    treePanel_->treeView()->expandAll();

    updateStatistics();
}

void MainWindow::updateStatistics()
{
    const int nodeCount = treeManager_.nodeCount();
    const int leafCount = treeManager_.leafCount();

    if (leafCount == 0)
    {
        statisticsPanel_->reset(nodeCount);
        return;
    }

    statisticsPanel_->setNodeCount(nodeCount);
    statisticsPanel_->setLeafCount(leafCount);
    statisticsPanel_->setMinimum(treeManager_.minimumValue());
    statisticsPanel_->setMaximum(treeManager_.maximumValue());
}