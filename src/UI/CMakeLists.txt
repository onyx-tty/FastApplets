cmake_minimum_required(VERSION 3.16)

project(UI VERSION 0.5.1 LANGUAGES CXX)

add_subdirectory(Enums)
add_subdirectory(Widgets)

# Internal structure
add_library(UI
        MainWindow.cpp)

target_include_directories(UI PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(UI PRIVATE
        UI::Enums
        UI::Widgets
        Config
        Log)

target_link_libraries(UI PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
