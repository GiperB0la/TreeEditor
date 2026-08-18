#pragma once
#include <QMainWindow>

#include "../widgets/ConnectionPanel/ConnectionPanel.hpp"
#include "../widgets/TreePanel/TreePanel.hpp"
#include "../widgets/ActionPanel/ActionPanel.hpp"
#include "../widgets/FilterPanel/FilterPanel.hpp"
#include "../widgets/FilePanel/FilePanel.hpp"
#include "../widgets/StatisticsPanel/StatisticsPanel.hpp"

#include "../../database/DatabaseManager.hpp"
#include "../../database/TreeRepository.hpp"
#include "../../tree/TreeManager.hpp"
#include "../models/TreeModel.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUi();
    void setupConnections();

    void onConnectClicked();

    void onAddNodeClicked();
    void onDeleteNodeClicked();
    void onAddLeafClicked();
    void onDeleteLeafClicked();
    void onEditClicked();

    void onFilterChanged();
    void onResetFilters();

    void onExportClicked();
    void onImportClicked();

    void updateStatistics();

private:
    ConnectionPanel *connectionPanel_ = nullptr;
    TreePanel *treePanel_ = nullptr;
    ActionPanel *actionPanel_ = nullptr;
    FilterPanel *filterPanel_ = nullptr;
    FilePanel* filePanel_ = nullptr;
    StatisticsPanel *statisticsPanel_ = nullptr;

    DatabaseManager databaseManager_;
    TreeRepository treeRepository_{ 
        databaseManager_.database()
    };
    TreeManager treeManager_{
        treeRepository_
    };
    TreeModel treeModel_;
};