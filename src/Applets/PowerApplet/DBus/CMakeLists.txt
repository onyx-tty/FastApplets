add_library(PowerApplet_DBus
        PowerActionManager.cpp)
add_library(PowerApplet::DBus ALIAS PowerApplet_DBus)

target_include_directories(PowerApplet_DBus PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_DBus PRIVATE
        DBus)

target_link_libraries(PowerApplet_DBus PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::DBus)
