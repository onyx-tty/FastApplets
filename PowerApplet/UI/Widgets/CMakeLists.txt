add_library(PowerApplet_UI_Widgets
        PowerButton.cpp)
add_library(PowerApplet::UI::Widgets ALIAS PowerApplet_UI_Widgets)

target_include_directories(PowerApplet_UI_Widgets PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_UI_Widgets PRIVATE
        Core::Shell)

target_link_libraries(PowerApplet_UI_Widgets PUBLIC
        Core::UI::Widgets
        Core::UI::Types
        Core::Config::KeysFile::Types
        Qt${QT_VERSION_MAJOR}::Widgets)
