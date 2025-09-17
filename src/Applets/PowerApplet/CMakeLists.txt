cmake_minimum_required(VERSION 3.16)

project(power-applet VERSION 0.11.0 LANGUAGES CXX)

# Set up the Qt environment
set(QT_VERSION_MAJOR 6)
find_package(Qt6 REQUIRED COMPONENTS Widgets DBus)
# TODO Qt5 fallback support
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets DBus)

# Internal structure
add_library(power_dbus
        DBus/PowerActionManager.cpp)
add_library(PowerApplet::DBus ALIAS power_dbus)
target_include_directories(power_dbus PRIVATE ${CMAKE_CURRENT_LIST_DIR}/DBus)
target_link_libraries(power_dbus PUBLIC Qt${QT_VERSION_MAJOR}::DBus)

add_library(power_ui
        UI/PowerMainWindow.cpp
        UI/CentralWidget.cpp
        UI/Widgets/PowerButton.cpp)
add_library(PowerApplet::UI ALIAS power_ui)
target_include_directories(power_ui PRIVATE ${CMAKE_CURRENT_LIST_DIR}/UI)
target_link_libraries(power_ui PRIVATE
        Core::UI
        PowerApplet::DBus # We call power actions on button press, so we need the DBus module
        Config::PowerApplet)
