cmake_minimum_required(VERSION 3.16)

project(Config VERSION 0.13.0 LANGUAGES CXX)

# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Requirements
find_package(tomlplusplus REQUIRED)

add_subdirectory(TOML)

add_library(Config
        Config.cpp
        Keys.cpp
        PrimaryButtonData.h
        ConfigMapper.cpp
        KeyMapper.cpp)

target_include_directories(Config PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config PRIVATE
        Config::TOML
        Core::Log
        UI::Enums
        CppUtils)

target_link_libraries(Config PUBLIC
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
