cmake_minimum_required(VERSION 3.16)

project(FastApplets)

# Applets for building
option(BUILD_POWERAPPLET "Include PowerApplet in the building process" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_VERSION_MAJOR 6)

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

set(GLOBAL_INCLUSIONS
        ${CMAKE_CURRENT_LIST_DIR}/src)

# Third-party libraries
if(BUILD_POWERAPPLET)
        add_subdirectory(
                external/CppUtils)
endif()

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
