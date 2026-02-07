cmake_minimum_required(VERSION 3.16)

project(config VERSION 0.3.2 LANGUAGES CXX)

# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Requirements
find_package(tomlplusplus REQUIRED)

add_library(config
        Config.cpp
        Keys.cpp
        PrimaryButtonData.h
        TomlConfigParser.cpp
        DefaultTomlConfig.cpp)
add_library(Config ALIAS config)
target_include_directories(config PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})
target_link_libraries(config PRIVATE
        CppUtils
        LogUtils)
target_link_libraries(config PUBLIC
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
