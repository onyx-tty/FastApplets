project(Shell VERSION 0.2.0 LANGUAGES CXX)

add_library(Shell
        ShellCommand.h
        ShellRunner.cpp)

target_include_directories(Shell PRIVATE
        ${SRC_DIR})

target_link_libraries(Shell PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
