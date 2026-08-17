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

    src/ui/MainWindow.hpp
    src/ui/MainWindow.cpp

    src/ui/ActionPanel/ActionPanel.hpp
    src/ui/ActionPanel/ActionPanel.cpp

    src/ui/ConnectionPanel/ConnectionPanel.hpp
    src/ui/ConnectionPanel/ConnectionPanel.cpp

    src/ui/FilePanel/FilePanel.hpp
    src/ui/FilePanel/FilePanel.cpp

    src/ui/FilterPanel/FilterPanel.hpp
    src/ui/FilterPanel/FilterPanel.cpp

    src/ui/StatisticsPanel/StatisticsPanel.hpp
    src/ui/StatisticsPanel/StatisticsPanel.cpp

    src/ui/TreePanel/TreePanel.hpp
    src/ui/TreePanel/TreePanel.cpp
    src/ui/TreePanel/TreeModel.hpp
    src/ui/TreePanel/TreeModel.cpp
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