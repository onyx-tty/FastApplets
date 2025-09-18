cmake_minimum_required(VERSION 3.16)

# Internal structure
add_library(curl
        Curl.cpp)
target_include_directories(curl PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(curl PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        CURL::libcurl)
