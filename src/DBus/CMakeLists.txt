project(dbus VERSION 0.1.1 LANGUAGES CXX)

# Requirements
# TODO Use QT_VERSION_MAJOR
find_package(Qt6 REQUIRED COMPONENTS DBus)

add_library(dbus
        DBusRequester.cpp
        DBusTarget.h)
add_library(DBus ALIAS dbus)
target_include_directories(dbus PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})
target_link_libraries(dbus PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::DBus)
target_link_libraries(dbus PRIVATE
        LogUtils)
