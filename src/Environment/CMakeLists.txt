add_library(environment
        RootResolver.cpp)
add_library(Environment ALIAS environment)
target_include_directories(environment PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(environment PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)