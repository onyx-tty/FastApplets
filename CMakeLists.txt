cmake_minimum_required(VERSION 3.16)

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

# Find packages
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(tomlplusplus REQUIRED)

if(BUILD_POWERAPPLET)
        find_package(Qt6 REQUIRED COMPONENTS DBus)
endif()

# Set up the Qt environment
set(QT_VERSION_MAJOR 6 CACHE STRING "Build using this version of Qt")

# Shared Directories
#+ Resources
add_subdirectory(
        resources
        Resources-build)

#+ Config
add_subdirectory(
        src/Config
        Config-build)

#+ Core
add_subdirectory(
        src/Core
        Core-build)

#+ DBus
add_subdirectory(
        src/DBus
        DBus-build)

#+ Environment
add_subdirectory(
        src/Environment
        Environment-build)

#+ Time
#add_subdirectory(
#        src/Time
#        Time-build)

# Applets
#+ PowerApplet
if(BUILD_POWERAPPLET)
        add_subdirectory(
                src/Applets/PowerApplet
                PowerApplet-build)
endif()

# Maintain support for both Qt6 and >=Qt5 to support a future expansion
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
        if (BUILD_POWERAPPLET)
                qt_add_executable(power-applet
                        MANUAL_FINALIZATION
                        ${POWERAPPLET_SOURCE_DIR}/main.cpp)
        endif()
else()
        if (BUILD_POWERAPPLET)
                add_executable(power-applet
                        ${POWERAPPLET_SOURCE_DIR}/main.cpp)
        endif()
endif()

# TODO Build type checking to disable the sanitizer for release versions
# Linked libraries and options
if(BUILD_POWERAPPLET)
        target_link_libraries(power-applet PRIVATE
                DBus
                Config
                Core::AllLibs
                PowerApplet::UI
                PowerApplet::DBus)
        target_compile_options(power-applet PRIVATE -fsanitize=address -fno-omit-frame-pointer -g)
        target_link_options(power-applet PRIVATE -fsanitize=address)
endif()

# Installation
include(GNUInstallDirs)

if(BUILD_POWERAPPLET)
        install(TARGETS power-applet
                BUNDLE DESTINATION .
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(QT_VERSION_MAJOR EQUAL 6)
        if(BUILD_POWERAPPLET)
                qt_finalize_executable(power-applet)
        endif()
endif()
