cmake_minimum_required(VERSION 3.16)

project(FastApplets)

# Source code directories
set(POWERAPPLET_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/Applets/PowerApplet CACHE PATH "Path to PowerApplet's source code")

# Applets to be built
option(BUILD_POWERAPPLET "Include PowerApplet in the building process" ON)

# Configure CMake
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# TODO Create a CMakeLists.txt file in src/ that does it there
# Specify the src/ directory as a global include for submodules
set(GLOBAL_INCLUDES
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/modules)

# Shared requirements
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# External modules
if(BUILD_POWERAPPLET)
        add_subdirectory(
                modules/CppUtils
                CppUtils-build)
endif()

# Internal modules
add_subdirectory(
        resources
        Resources-build)

add_subdirectory(
        src/Core
        Core-build
)

add_subdirectory(
        src/Config
        Config-build)

add_subdirectory(
        src/UI
        UI-build)

add_subdirectory(
        src/DBus
        DBus-build)

add_subdirectory(
        src/Environment
        Environment-build)

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

# TODO Build type checking to disable the sanitizer for release versions
# Target linking and configuration
if(BUILD_POWERAPPLET)
        target_include_directories(PowerApplet PRIVATE
                ${GLOBAL_INCLUDES})
        target_link_libraries(PowerApplet PRIVATE
                PowerApplet::Deps)
        target_compile_options(PowerApplet PRIVATE -fsanitize=address -fno-omit-frame-pointer -g)
        target_link_options(PowerApplet PRIVATE -fsanitize=address)
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
