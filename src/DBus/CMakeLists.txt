add_library(dbus
        DBusRequester.cpp
        DBusTarget.h)
add_library(DBus ALIAS dbus)
target_include_directories(dbus PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(dbus PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::DBus)
