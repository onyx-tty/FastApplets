add_library(power_dbus
        PowerActionManager.cpp)
add_library(PowerApplet::DBus ALIAS power_dbus)
target_include_directories(power_dbus PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${GLOBAL_INCLUDES})
target_link_libraries(power_dbus PRIVATE
        LogUtils
        DBus)
target_link_libraries(power_dbus PUBLIC
        Qt${QT_VERSION_MAJOR}::DBus)

