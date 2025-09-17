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

if(BUILD_WEATHERAPPLET)
        add_library(config_weather_applet
                WeatherLayout.cpp)
        add_library(Config::WeatherApplet ALIAS config_weather_applet)
        target_include_directories(config_weather_applet PUBLIC ${CMAKE_CURRENT_LIST_DIR})
        target_link_libraries(config_weather_applet PUBLIC Config::Shared)
endif()
