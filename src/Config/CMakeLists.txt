cmake_minimum_required(VERSION 3.16)

project(Config VERSION 0.28.3 LANGUAGES CXX)

# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Requirements
find_package(tomlplusplus REQUIRED)

add_subdirectory(Config)
add_subdirectory(Resolvers)
add_subdirectory(Keys)
add_subdirectory(TOML)

add_library(Config)

target_include_directories(Config PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config PRIVATE
        Config::Config
        Config::Resolvers
        Config::Keys
        Config::TOML
        Core::Log
        UI::Widgets
        CppUtils)

target_link_libraries(Config PUBLIC
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
