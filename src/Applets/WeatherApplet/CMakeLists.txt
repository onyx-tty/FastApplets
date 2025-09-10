cmake_minimum_required(VERSION 3.16)

project(weather-applet VERSION 0.1 LANGUAGES CXX)

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
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# TODO Qt5 fallback support
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

# Configure TraverseJSON to enable Qt support
set(TRAVERSEJSON_ENABLE_QT ON CACHE BOOL "Enable Qt in TraverseJSON")

# TODO Use FetchContent to skip git submodule and additional work for the user
# Submodules
add_subdirectory(
        ../../../modules/TraverseJSON
        ../../../build/TraverseJSON-build)

# Core
add_subdirectory(
        ../..
        ../../../build/Core-build)

# Internal structure
add_library(weather_main_window
        WeatherMainWindow.cpp
        CentralWidget.cpp)
add_library(WeatherApplet::MainWindow ALIAS weather_main_window)
target_include_directories(weather_main_window PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(weather_main_window PRIVATE Core::MainWindow)

add_library(weather_api
        API/OpenWeatherAPI.cpp
        API/WeatherData.cpp
        API/WeatherParser.cpp)
add_library(WeatherApplet::API ALIAS weather_api)
target_include_directories(weather_api PRIVATE ${CMAKE_CURRENT_LIST_DIR}/API)
target_link_libraries(weather_api PRIVATE TraverseJSON)
target_link_libraries(weather_api PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

add_library(weather_utils
        Utils/PresentTimeManager.cpp)
add_library(WeatherApplet::Utils ALIAS weather_utils)
target_include_directories(weather_utils PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Utils)
target_link_libraries(weather_utils PRIVATE WeatherConfig)

# Maintain support for both Qt6 and >=Qt5 to support a future expansion
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
        qt_add_executable(weather-applet
                MANUAL_FINALIZATION
                main.cpp)
else()
        add_executable(weather-applet
                main.cpp)
endif()

# Linked libraries
target_link_libraries(weather-applet PRIVATE
        Core::AllLibs
        WeatherApplet::MainWindow
        WeatherApplet::API
        WeatherApplet::Utils)

# Installation
include(GNUInstallDirs)
install(TARGETS weather-applet
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(weather-applet)
endif()
