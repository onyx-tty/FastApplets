project(DBus VERSION 0.1.5 LANGUAGES CXX)

# Requirements
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus)

add_library(DBus
        DBusRequester.cpp
        DBusTarget.h)

target_include_directories(DBus PRIVATE
        ${SRC_DIR})

target_link_libraries(DBus PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::DBus)
