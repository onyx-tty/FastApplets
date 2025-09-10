cmake_minimum_required(VERSION 3.16)

# Set up the Qt environment
set(QT_VERSION_MAJOR 6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# TODO Qt5 fallback support
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui)

# Find external dependencies
find_package(CURL REQUIRED)

# Enable Qt support for QObject MOC and resource files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Interal structure
add_library(core_main_window
        MainWindow.cpp)
add_library(Core::MainWindow ALIAS core_main_window)
target_include_directories(core_main_window PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(core_main_window PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

add_library(core_widgets
        Widgets/Button.cpp)
add_library(Core::Widgets ALIAS core_widgets)
target_include_directories(core_widgets PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Widgets)
target_link_libraries(core_widgets PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

add_library(core_api
        API/Curl.cpp)
add_library(Core::API ALIAS core_api)
target_include_directories(core_api PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/API)
target_link_libraries(core_api PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        CURL::libcurl)

add_library(core_utils
        Utils/Time.cpp
        Utils/TimeConstants.h)
add_library(Core::Utils ALIAS core_utils)
target_include_directories(core_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Utils)
target_link_libraries(core_utils PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

# TODO TOML or separate config CMake file
add_library(core_config
        Config/SharedLayout.cpp
        Config/SharedKeybindings.cpp)
add_library(CoreConfig ALIAS core_config)
target_include_directories(core_config PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Config)
target_link_libraries(core_config PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)

add_library(power_config
        Config/PowerKeybindings.cpp
        Config/PowerLayout.cpp)
add_library(PowerConfig ALIAS power_config)
target_include_directories(power_config PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Config)
target_link_libraries(power_config PUBLIC
        CoreConfig
        Qt${QT_VERSION_MAJOR}::Widgets)

add_library(weather_config
        Config/WeatherLayout.cpp)
add_library(WeatherConfig ALIAS weather_config)
target_include_directories(weather_config PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Config)
target_link_libraries(weather_config PUBLIC
        CoreConfig
        Qt${QT_VERSION_MAJOR}::Widgets)

add_library(core_all_libs INTERFACE)
add_library(Core::AllLibs ALIAS core_all_libs)
target_link_libraries(core_all_libs INTERFACE
        Core::MainWindow
        Core::Widgets
        Core::API
        Core::Utils)
