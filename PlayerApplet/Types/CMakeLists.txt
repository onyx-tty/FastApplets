add_library(PlayerApplet_Types INTERFACE)
add_library(PlayerApplet::Types ALIAS PlayerApplet_Types)

target_include_directories(PlayerApplet_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PlayerApplet_Types INTERFACE
        Core::Applets::Types # type & Traits
        Qt${QT_VERSION_MAJOR}::Core)
