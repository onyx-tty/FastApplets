cmake_minimum_required(VERSION 3.16)

project(Config VERSION 0.11.7 LANGUAGES CXX)

# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Requirements
find_package(tomlplusplus REQUIRED)

add_library(Config
        Config.cpp
        Keys.cpp
        PrimaryButtonData.h
        ConfigLocator.cpp
        ConfigParser.cpp
        ConfigMapper.cpp)

target_include_directories(Config PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config PRIVATE
        CppUtils
        Core::Log
        UI::Enums)

target_link_libraries(Config PUBLIC
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
