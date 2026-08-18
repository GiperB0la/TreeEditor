cmake_minimum_required(VERSION 3.21)

project(TreeEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql Test)

qt_standard_project_setup()

add_executable(TreeEditor
    src/Main.cpp


    src/database/DatabaseManager.hpp
    src/database/DatabaseManager.cpp
    src/database/TreeRepository.hpp
    src/database/TreeRepository.cpp


    src/tree/TreeItem.hpp
    src/tree/TreeManager.hpp
    src/tree/TreeManager.cpp


    src/ui/MainWindow/MainWindow.hpp
    src/ui/MainWindow/MainWindow.cpp


    src/ui/widgets/ActionPanel/ActionPanel.hpp
    src/ui/widgets/ActionPanel/ActionPanel.cpp

    src/ui/widgets/ConnectionPanel/ConnectionPanel.hpp
    src/ui/widgets/ConnectionPanel/ConnectionPanel.cpp

    src/ui/widgets/FilePanel/FilePanel.hpp
    src/ui/widgets/FilePanel/FilePanel.cpp

    src/ui/widgets/FilterPanel/FilterPanel.hpp
    src/ui/widgets/FilterPanel/FilterPanel.cpp

    src/ui/widgets/StatisticsPanel/StatisticsPanel.hpp
    src/ui/widgets/StatisticsPanel/StatisticsPanel.cpp

    src/ui/widgets/TreePanel/TreePanel.hpp
    src/ui/widgets/TreePanel/TreePanel.cpp


    src/ui/models/TreeModel.hpp
    src/ui/models/TreeModel.cpp

    src/ui/models/TreeFilterProxyModel.hpp
    src/ui/models/TreeFilterProxyModel.cpp
)

target_link_libraries(TreeEditor
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
)

enable_testing()

add_executable(TreeManagerTest
    tests/TreeManagerTest.cpp

    src/database/TreeRepository.hpp
    src/database/TreeRepository.cpp

    src/tree/TreeItem.hpp
    src/tree/TreeManager.hpp
    src/tree/TreeManager.cpp
)

target_link_libraries(TreeManagerTest
    PRIVATE
        Qt6::Core
        Qt6::Sql
        Qt6::Test
)

add_test(
    NAME TreeManagerTest
    COMMAND TreeManagerTest
)