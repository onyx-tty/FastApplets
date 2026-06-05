project(Core_Shell VERSION 0.3.4 LANGUAGES CXX)

add_library(Core_Shell
        ShellRunner.cpp)
add_library(Core::Shell ALIAS Core_Shell)

target_include_directories(Core_Shell PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Shell PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)

