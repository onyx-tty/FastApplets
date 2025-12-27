project(environment VERSION 0.1.1 LANGUAGES CXX)

add_library(environment
        RootResolver.cpp)
add_library(Environment ALIAS environment)
target_include_directories(environment PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})
target_link_libraries(environment PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(environment PRIVATE LogUtils)
