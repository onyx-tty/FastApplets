add_subdirectory(Widgets)

add_library(ActionApplet_UI
        ActionMainWindow.cpp
        ActionCentralWidget.cpp)
add_library(ActionApplet::UI ALIAS ActionApplet_UI)

target_include_directories(ActionApplet_UI PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ActionApplet_UI PRIVATE
        ActionApplet::UI::Widgets
        Core::UI::Widgets
        Core::UI::Types
        Core::Config::ConfigFile::ActionApplet
        Core::Config::KeysFile::ActionApplet)

target_link_libraries(ActionApplet_UI PUBLIC
        Core::UI
        Core::Config::KeysFile::Types
        Qt${QT_VERSION_MAJOR}::Widgets)
