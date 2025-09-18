cmake_minimum_required(VERSION 3.16)

project(core VERSION 0.1.0 LANGUAGES CXX)

# Internal structure
add_library(core_ui
        UI/MainWindow.cpp
        UI/Widgets/Button.cpp)
add_library(Core::UI ALIAS core_ui)
target_include_directories(core_ui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/UI)
target_link_libraries(core_ui PUBLIC
        Config::Shared
        Qt${QT_VERSION_MAJOR}::Widgets)

add_library(core_all_libs INTERFACE)
add_library(Core::AllLibs ALIAS core_all_libs)
target_link_libraries(core_all_libs INTERFACE
        Core::UI)
