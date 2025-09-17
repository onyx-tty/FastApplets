cmake_minimum_required(VERSION 3.16)

# Enable Qt support for Qt's resource files
set(CMAKE_AUTORCC ON)

# TODO TOML or separate config CMake file
add_library(config_shared
        SharedLayout.cpp
        SharedKeybindings.cpp)
add_library(Config::Shared ALIAS config_shared)
target_include_directories(config_shared PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(config_shared PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

if(BUILD_POWERAPPLET)
        add_library(config_power_applet
                PowerLayout.cpp
                PowerKeybindings.cpp
                ${POWERAPPLET_RESOURCES})
        add_library(Config::PowerApplet ALIAS config_power_applet)
        target_include_directories(config_power_applet PUBLIC ${CMAKE_CURRENT_LIST_DIR})
        target_link_libraries(config_power_applet PUBLIC Config::Shared)
endif()
