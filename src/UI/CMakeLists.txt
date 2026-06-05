cmake_minimum_required(VERSION 3.16)

project(UI VERSION 0.5.12 LANGUAGES CXX)

add_subdirectory(Widgets)
add_subdirectory(Types)

# Internal structure
add_library(UI
        MainWindow.cpp)

target_include_directories(UI PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(UI PRIVATE
        UI::Types
        UI::Widgets
        Config::ConfigFile::Properties)

target_link_libraries(UI PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
