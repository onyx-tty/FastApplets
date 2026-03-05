project(Environment VERSION 0.1.0 LANGUAGES CXX)

add_library(Environment
        ShellCommand.h
        ShellRunner.cpp)

target_include_directories(Environment PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Environment PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
