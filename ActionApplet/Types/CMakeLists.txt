add_library(ActionApplet_Types INTERFACE)
add_library(ActionApplet::Types ALIAS PowerApplet_Types)

target_include_directories(ActionApplet_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_Types INTERFACE
        ActionApplet::UI::Widgets # ActionButton
        Core::Config::ConfigFile::ActionApplet # ActionAppletConfig
        Core::Config::KeysFile::ActionApplet # ActionAppletKeys
        Core::UI::Widgets # ActionButtonParams
        Core::UI::Types # ButtonType
        Core::Applets::Types) # AppletType & AppletTraits
