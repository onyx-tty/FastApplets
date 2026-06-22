add_library(ActionApplet_UI_Widgets
        ActionButton.cpp)
add_library(ActionApplet::UI::Widgets ALIAS ActionApplet_UI_Widgets)

target_include_directories(ActionApplet_UI_Widgets PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ActionApplet_UI_Widgets PRIVATE
        Core::Shell
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(ActionApplet_UI_Widgets PUBLIC
        Core::UI::Widgets
        Core::Config::KeysFile::Types
        Qt${QT_VERSION_MAJOR}::Core)
