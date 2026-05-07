project(Shell VERSION 0.3.3 LANGUAGES CXX)

add_library(Shell
        ShellRunner.cpp)

target_include_directories(Shell PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Shell PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)

