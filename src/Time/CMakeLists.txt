cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_library(time
        Time.cpp
        TimeConstants.h)
target_include_directories(time PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Time)
target_link_libraries(time PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
