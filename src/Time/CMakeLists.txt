cmake_minimum_required(VERSION 3.16)

project(Time VERSION 0.2.0 LANGUAGES CXX)

add_library(Time
        Time.cpp
        TimeConstants.h)

target_include_directories(Time PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Time PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Time PRIVATE Core::Log)
