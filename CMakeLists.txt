cmake_minimum_required(VERSION 3.16)

project(FastApplets)

# Applets to be built
option(BUILD_POWERAPPLET "Include PowerApplet in the building process" ON)

# Configure CMake
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# clang-tidy
if(ENABLE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY
                clang-tidy;
                -header-filter=.;)
endif()

# Enable Qt support for QObject MOC
set(CMAKE_AUTOMOC ON)

# Enable Qt support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build shared by default
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Build unit tests
option(BUILD_TESTS "Build unit tests" ON)

# Set up the Qt environment
set(QT_VERSION_MAJOR 6 CACHE STRING "Build using this version of Qt")

# Specify global inclusions shared by all modules
set(GLOBAL_INCLUSIONS
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/external)

# Shared requirements
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# External modules
if(BUILD_POWERAPPLET)
        if(LOG_DEBUG)
                set(LOG_DEBUG ON CACHE BOOL "Print debug-level logs" FORCE)
        endif()

        add_subdirectory(
                external/CppUtils)
endif()

# Internal modules
add_subdirectory(
        resources)

add_subdirectory(
        src/Log)

add_subdirectory(
        src/Config)

add_subdirectory(
        src/Shell)

add_subdirectory(
        src/UI)

add_subdirectory(
        src/DBus)

add_subdirectory(
        src/Time)

# Applets
if(BUILD_POWERAPPLET)
        add_subdirectory(
                src/Applets/PowerApplet)
endif()

# Tests
if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
endif()

# Installation
include(GNUInstallDirs)
