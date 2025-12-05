cmake_minimum_required(VERSION 3.16)

project(PowerApplet VERSION 0.17.2 LANGUAGES CXX)

# Internal structure
add_library(power_dbus
        DBus/PowerActionManager.cpp)
add_library(PowerApplet::DBus ALIAS power_dbus)
target_include_directories(power_dbus PRIVATE ${CMAKE_CURRENT_LIST_DIR}/DBus)
target_link_libraries(power_dbus PUBLIC
        Qt${QT_VERSION_MAJOR}::DBus
        DBus)

add_library(power_ui
        ${POWERAPPLET_RESOURCES}
        UI/PowerMainWindow.cpp
        UI/PowerCentralWidget.cpp
        UI/Widgets/PowerButton.cpp)
add_library(PowerApplet::UI ALIAS power_ui)
target_include_directories(power_ui PRIVATE ${CMAKE_CURRENT_LIST_DIR}/UI)
target_link_libraries(power_ui PRIVATE
        Config
        UI
        PowerApplet::DBus) # Power actions on button press, so DBus module is needed

# Internal dependencies
add_library(power_deps)
add_library(PowerApplet::Deps ALIAS power_deps)
target_link_libraries(power_deps INTERFACE
                Core
                Config
                DBus
                UI
                PowerApplet::UI
                PowerApplet::DBus)
