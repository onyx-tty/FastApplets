cmake_minimum_required(VERSION 3.16)

project(config VERSION 0.2.0 LANGUAGES CXX)

# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

add_library(config
        Config.cpp
        Keys.cpp
        PrimaryButtonData.h
        TomlConfigParser.cpp)
add_library(Config ALIAS config)
target_include_directories(config PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(config PUBLIC
        CppUtils
        Environment
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
