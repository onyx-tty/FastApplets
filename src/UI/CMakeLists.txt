cmake_minimum_required(VERSION 3.16)

project(UI VERSION 0.5.2 LANGUAGES CXX)

add_subdirectory(Enums)
add_subdirectory(Widgets)

# Internal structure
add_library(UI
        MainWindow.cpp)

target_include_directories(UI PRIVATE
        ${SRC_DIR})

target_link_libraries(UI PRIVATE
        UI::Enums
        UI::Widgets
        Config)

target_link_libraries(UI PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
