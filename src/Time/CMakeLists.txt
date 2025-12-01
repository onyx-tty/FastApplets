cmake_minimum_required(VERSION 3.16)

project(time VERSION 0.1.0 LANGUAGES CXX)

add_library(time
        Time.cpp
        TimeConstants.h)
target_include_directories(time PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Time)
target_link_libraries(time PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
