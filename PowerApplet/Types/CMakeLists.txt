add_library(PowerApplet_Types INTERFACE)
add_library(PowerApplet::Types ALIAS PowerApplet_Types)

target_include_directories(PowerApplet_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_Types INTERFACE
        Core::UI::Types # button_type
        Core::Applets::Types # AppletType & AppletTraits
        Qt${QT_VERSION_MAJOR}::Core)
