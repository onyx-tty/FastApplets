cmake_minimum_required(VERSION 3.16)

project(UI VERSION 0.2.4 LANGUAGES CXX)

# Internal structure
add_library(UI
        MainWindow.cpp
        Widgets/Button.cpp)

target_include_directories(UI PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})

target_link_libraries(UI PRIVATE
        Config
        Core::Log)

target_link_libraries(UI PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
