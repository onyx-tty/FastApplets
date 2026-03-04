cmake_minimum_required(VERSION 3.16)

project(PowerApplet VERSION 0.21.7 LANGUAGES CXX)

add_subdirectory(DBus)
add_subdirectory(UI)

# Internal dependencies
add_library(PowerApplet_Deps)
add_library(PowerApplet::Deps ALIAS PowerApplet_Deps)

target_link_libraries(PowerApplet_Deps INTERFACE
        Core
        Config
        DBus
        UI
        PowerApplet::UI
        PowerApplet::DBus)
