cmake_minimum_required(VERSION 3.16)

project(weather-applet VERSION 0.1 LANGUAGES CXX)

# Internal structure
add_library(weather_data
        Data/WeatherData.cpp)
add_library(WeatherApplet::Data ALIAS weather_data)
target_include_directories(weather_data PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Data)
target_link_libraries(weather_data PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)

add_library(weather_ui
        UI/WeatherMainWindow.cpp
        UI/CentralWidget.cpp)
add_library(WeatherApplet::UI ALIAS weather_ui)
target_include_directories(weather_ui PRIVATE ${CMAKE_CURRENT_LIST_DIR}/UI)
target_link_libraries(weather_ui PRIVATE
        WeatherApplet::Data
        Core::UI
        Config::WeatherApplet)

add_library(weather_parsers
        Parsers/OpenWeatherParser.cpp)
add_library(WeatherApplet::Parsers ALIAS weather_parsers)
target_include_directories(weather_parsers PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Parsers)
target_link_libraries(weather_parsers PRIVATE
        Config::WeatherApplet
        time
        TraverseJSON)
target_link_libraries(weather_parsers PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_library(weather_networking
        Networking/OpenWeatherClient.cpp)
add_library(WeatherApplet::Networking ALIAS weather_networking)
target_include_directories(weather_networking PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Networking)
target_link_libraries(weather_networking PRIVATE CurlOOP)

add_library(weather_time
        Time/ForecastBlocManager.cpp)
add_library(WeatherApplet::Time ALIAS weather_time)
target_include_directories(weather_time PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Time)
target_link_libraries(weather_time PRIVATE Config::WeatherApplet)
