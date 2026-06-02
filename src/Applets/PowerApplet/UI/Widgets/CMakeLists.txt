add_library(PowerApplet_UI_Widgets
        PowerButton.cpp)
add_library(PowerApplet::UI::Widgets ALIAS PowerApplet_UI_Widgets)

target_include_directories(PowerApplet_UI_Widgets PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_UI_Widgets PRIVATE
        PowerApplet::Types
        UI::Widgets
        UI::Types
        Config::ConfigManager
        Shell
        AppletTypes)

target_link_libraries(PowerApplet_UI_Widgets PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
