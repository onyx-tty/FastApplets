add_library(ActionApplet_Types INTERFACE)
add_library(ActionApplet::Types ALIAS ActionApplet_Types)

target_include_directories(ActionApplet_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ActionApplet_Types INTERFACE
        Core::Config::ConfigFile::ActionApplet # ActionAppletConfig
        Core::Config::KeysFile::ActionApplet # ActionAppletKeys
        Core::Applets::Types) # AppletType & AppletTraits
