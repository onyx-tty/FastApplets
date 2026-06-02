add_library(PowerApplet_Types)
add_library(PowerApplet::Types ALIAS PowerApplet_Types)

target_include_directories(PowerApplet_Types PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_Types PRIVATE
        PowerApplet::UI::Widgets # PowerButton
        Config::ConfigFile::PowerApplet # PowerAppletConfig
        Config::KeysFile::PowerApplet # PowerAppletKeys
        UI::Widgets # PowerButtonParams
        UI::Types # ButtonType
        AppletTypes) # AppletType & AppletTraits
