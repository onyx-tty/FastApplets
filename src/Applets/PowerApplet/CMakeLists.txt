cmake_minimum_required(VERSION 3.16)

project(PowerApplet VERSION 0.17.9 LANGUAGES CXX)

add_subdirectory(DBus)
add_subdirectory(UI)

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
