cmake_minimum_required(VERSION 3.16)

project(FastApplets)

# Applets for building
option(BUILD_POWERAPPLET "Include PowerApplet in the building process" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_VERSION_MAJOR 6)

# Install and use CPM to resolve conflicting and duplicated CppUtils
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=f3a6dcc6a04ce9e7f51a127307fa4f699fb2bade357a8eb4c5b45df76e1dc6a5)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# clang-tidy
if(ENABLE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY
                clang-tidy;
                -header-filter=.;)
endif()

# Export compile_commands.json to ensure language servers work properly
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build shared by default
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_TESTS "Build unit tests" ON)
set(CPPUTILS_ENABLE_QT ON)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_DIR ${ROOT_DIR}/src)

# Third-party libraries
if(BUILD_POWERAPPLET)
        CPMAddPackage("gh:onyx-tty/CppUtils#926ee69")
        CPMAddPackage("gh:onyx-tty/TomlQt#ae41b79")
endif()

#find_package(TomlQt REQUIRED)

# Internals
add_subdirectory(
        resources)

add_subdirectory(
        src/Config)

add_subdirectory(
        src/Shell)

add_subdirectory(
        src/UI)

add_subdirectory(
        src/DBus)

if(BUILD_POWERAPPLET)
        add_subdirectory(
                src/Applets/PowerApplet)
endif()

#if(BUILD_TESTS)
#        enable_testing()
#        add_subdirectory(tests)
#endif()


include(GNUInstallDirs)
