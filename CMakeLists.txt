cmake_minimum_required(VERSION 3.16)

set(WEATHERAPPLET_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/Applets/WeatherApplet CACHE PATH "Path to WeatherApplet's source code")

option(BUILD_WEATHERAPPLET "Include WeatherApplet in the building process" ON)

# Configure CMake
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt support for QObject MOC
set(CMAKE_AUTOMOC ON)

# Enable Qt support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Build shared by default
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Configure TraverseJSON to enable Qt support
set(TRAVERSEJSON_ENABLE_QT ON CACHE BOOL "Enable Qt in TraverseJSON" FORCE)

# Find packages
find_package(Qt6 REQUIRED COMPONENTS Widgets)


# TODO Build type checking to disable the sanitizer for release versions
# Set up the Qt environment
set(QT_VERSION_MAJOR 6 CACHE STRING "Build using this version of Qt")

# Shared Directories
#+ Config
add_subdirectory(
        src/Config
        build/Config-build)

#+ Core
add_subdirectory(
        src/Core
        build/Core-build)

#+ Networking
if(BUILD_WEATHERAPPLET)
        add_subdirectory(
                src/Networking
                build/Networking-build)
endif()

#+ Time
if(BUILD_WEATHERAPPLET)
        add_subdirectory(
                src/Time
                build/Time-build)
endif()

# Applets
#+ WeatherApplet
if(BUILD_WEATHERAPPLET)
        add_subdirectory(
                src/Applets/WeatherApplet
                build/WeatherApplet-build)
endif()

# Modules
#+ TraverseJSON
if(BUILD_WEATHERAPPLET)
        add_subdirectory(
                modules/TraverseJSON
                build/TraverseJSON-build)
endif()

# Maintain support for both Qt6 and >=Qt5 to support a future expansion
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
        if (BUILD_WEATHERAPPLET)
                qt_add_executable(weather-applet
                        MANUAL_FINALIZATION
                        ${WEATHERAPPLET_SOURCE_DIR}/main.cpp)
        endif()
else()
        if(BUILD_WEATHERAPPLET)
                add_executable(weather-applet
                        ${WEATHERAPPLET_SOURCE_DIR}/main.cpp)
        endif()
endif()

# Linked libraries
if(BUILD_WEATHERAPPLET)
        target_link_libraries(weather-applet PRIVATE
                Core::AllLibs
                TraverseJSON
                WeatherApplet::UI
                WeatherApplet::Parsers
                WeatherApplet::Networking
                WeatherApplet::Data
                WeatherApplet::Time)
        target_compile_options(weather-applet PRIVATE -fsanitize=address -fno-omit-frame-pointer -g)
        target_link_options(weather-applet PRIVATE -fsanitize=address)
endif()

# Installation
include(GNUInstallDirs)

if(BUILD_WEATHERAPPLET)
        install(TARGETS weather-applet
                BUNDLE DESTINATION .
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(QT_VERSION_MAJOR EQUAL 6)
        if(BUILD_WEATHERAPPLET)
                qt_finalize_executable(weather-applet)
        endif()
endif()
