cmake_minimum_required(VERSION 3.16)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Set up the Qt environment
set(QT_VERSION_MAJOR 6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# TODO Qt5 fallback support
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui)

# External dependencies
find_package(CURL REQUIRED)

# Internal structure
add_library(curl
        Curl.cpp)
target_include_directories(curl PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(curl PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        CURL::libcurl)
