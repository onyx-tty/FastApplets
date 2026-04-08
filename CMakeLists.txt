cmake_minimum_required(VERSION 3.16)

project(FastApplets)

# Source code directories
set(POWERAPPLET_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/Applets/PowerApplet CACHE PATH "Path to PowerApplet's source code")

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
                external/CppUtils
                CppUtils-build)
endif()

# Internal modules
add_subdirectory(
        resources
        Resources-build)

add_subdirectory(
        src/Core
        Core-build)

add_subdirectory(
        src/Config
        Config-build)

add_subdirectory(
        src/Environment
        Environment-build)

add_subdirectory(
        src/UI
        UI-build)

add_subdirectory(
        src/DBus
        DBus-build)

add_subdirectory(
        src/Time
        Time-build)

# Applets
if(BUILD_POWERAPPLET)
        add_subdirectory(
                src/Applets/PowerApplet
                PowerApplet-build)
endif()

# Executables
if (BUILD_POWERAPPLET)
        qt_add_executable(PowerApplet
                MANUAL_FINALIZATION
                ${POWERAPPLET_SOURCE_DIR}/main.cpp)
endif()

# Target linking and configuration
if(BUILD_POWERAPPLET)
        target_include_directories(PowerApplet PRIVATE
                ${GLOBAL_INCLUSIONS})

        target_link_libraries(PowerApplet PRIVATE
                PowerApplet::Deps)

        if (ENABLE_ASAN OR ENABLE_LSAN OR ENABLE_UBSAN)
                target_compile_options(PowerApplet PRIVATE -fno-omit-frame-pointer -g)
        endif()

        if(ENABLE_ASAN)
                target_compile_options(PowerApplet PRIVATE -fsanitize=address)
                target_link_options(PowerApplet PRIVATE -fsanitize=address)
        endif()

        if(ENABLE_LSAN)
                target_compile_options(PowerApplet PRIVATE -fsanitize=leak)
                target_link_options(PowerApplet PRIVATE -fsanitize=leak)
        endif()

        if(ENABLE_UBSAN)
                target_compile_options(PowerApplet PRIVATE -fsanitize=undefined)
                target_link_options(PowerApplet PRIVATE -fsanitize=undefined)
        endif()
endif()

# Tests
if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
endif()

# Installation
include(GNUInstallDirs)

if(BUILD_POWERAPPLET)
        install(TARGETS PowerApplet
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# Finalization
if(BUILD_POWERAPPLET)
        qt_finalize_executable(PowerApplet)
endif()
