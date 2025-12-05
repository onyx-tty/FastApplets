cmake_minimum_required(VERSION 3.16)

project(ui VERSION 0.2.0 LANGUAGES CXX)

# Internal structure
add_library(ui
        MainWindow.cpp
        Widgets/Button.cpp)
add_library(UI ALIAS ui)
target_include_directories(ui PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})
target_link_libraries(ui PUBLIC
        Config
        Qt${QT_VERSION_MAJOR}::Widgets)
