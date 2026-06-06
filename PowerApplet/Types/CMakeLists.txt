add_library(PowerApplet_Types INTERFACE)
add_library(PowerApplet::Types ALIAS PowerApplet_Types)

target_include_directories(PowerApplet_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_Types INTERFACE
        PowerApplet::UI::Widgets # PowerButton
        Core::Config::ConfigFile::PowerApplet # PowerAppletConfig
        Core::Config::KeysFile::PowerApplet # PowerAppletKeys
        Core::UI::Widgets # PowerButtonParams
        Core::UI::Types # ButtonType
        Core::AppletTypes) # AppletType & AppletTraits
