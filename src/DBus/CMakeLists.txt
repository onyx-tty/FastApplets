project(DBus VERSION 0.1.7 LANGUAGES CXX)

# Requirements
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus)

add_library(DBus
        DBusRequester.cpp
        DBusTarget.h)

target_include_directories(DBus PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(DBus PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::DBus)
