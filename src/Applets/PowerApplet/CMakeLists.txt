cmake_minimum_required(VERSION 3.16)

project(power-applet VERSION 0.10.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Enable Qt support for QObject MOC and resource files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Configure CMake further
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# TODO Build type checking to disable sanitizer for release versions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

# Set up the Qt environment
set(QT_VERSION_MAJOR 6)
find_package(Qt6 REQUIRED COMPONENTS Widgets DBus)
# TODO Qt5 fallback support
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets DBus)

# Core
add_subdirectory(
        ../..
        ../../../build/Core-build)

# Internal structure
add_library(power_main_window
        PowerMainWindow.cpp
        CentralWidget.cpp)
add_library(PowerApplet::MainWindow ALIAS power_main_window)
target_include_directories(power_main_window PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(power_main_window PRIVATE
        Core::MainWindow
        PowerConfig)

add_library(power_dbus
        DBus/PowerActionManager.cpp)
add_library(PowerApplet::DBus ALIAS power_dbus)
target_include_directories(power_dbus PRIVATE ${CMAKE_CURRENT_LIST_DIR}/DBus)
target_link_libraries(power_dbus PUBLIC Qt${QT_VERSION_MAJOR}::DBus)

add_library(power_widgets
        Widgets/PowerButton.cpp)
add_library(PowerApplet::Widgets ALIAS power_widgets)
target_include_directories(power_widgets PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Widgets)
target_link_libraries(power_widgets PRIVATE
        Core::Widgets
        PowerApplet::DBus) # We call power actions on button press, it's needed

add_library(power_resources
        Data/Icons.qrc)
add_library(PowerApplet::Resources ALIAS power_resources)
target_include_directories(power_resources PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Data)

# Maintain support for both Qt6 and >=Qt5 to support a future expansion
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
        qt_add_executable(power-applet
                MANUAL_FINALIZATION
                main.cpp)
else()
        add_executable(power-applet
                main.cpp)
endif()

# Linked libraries
target_link_libraries(power-applet PRIVATE
        Core::AllLibs
        PowerApplet::MainWindow
        PowerApplet::Widgets
        PowerApplet::DBus
        PowerApplet::Resources)

# Installation
include(GNUInstallDirs)
install(TARGETS power-applet
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(power-applet)
endif()
