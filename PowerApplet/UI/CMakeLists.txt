add_subdirectory(Widgets)

add_library(PowerApplet_UI INTERFACE)
add_library(PowerApplet::UI ALIAS PowerApplet_UI)

target_include_directories(PowerApplet_UI INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_UI INTERFACE
        PowerApplet::UI::Widgets)
