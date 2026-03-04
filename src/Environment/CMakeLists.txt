project(Environment VERSION 0.0.2 LANGUAGES CXX)

add_library(Environment
        ShellRunner.cpp)

target_include_directories(Environment PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Environment PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
