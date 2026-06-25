add_subdirectory(Widgets)

add_library(ActionApplet_UI INTERFACE)
add_library(ActionApplet::UI ALIAS ActionApplet_UI)

target_include_directories(ActionApplet_UI INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ActionApplet_UI INTERFACE
        ActionApplet::UI::Widgets)
