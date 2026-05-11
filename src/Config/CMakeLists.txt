cmake_minimum_required(VERSION 3.16)

project(Config VERSION 0.42.4 LANGUAGES CXX)

# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Requirements
find_package(tomlplusplus REQUIRED)

add_subdirectory(ConfigFile)
add_subdirectory(KeysFile)
add_subdirectory(FileLocator)
add_subdirectory(TomlParser)
add_subdirectory(Resolver)
add_subdirectory(Types)

add_library(Config)

target_include_directories(Config PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config PRIVATE
        Config::ConfigFile
        Config::KeysFile
        Config::FileLocator
        Config::TomlParser
        Config::Resolver
        Config::Types
        UI::Widgets)

target_link_libraries(Config PUBLIC
        CppUtils::Log::Qt
        CppUtils::String
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
