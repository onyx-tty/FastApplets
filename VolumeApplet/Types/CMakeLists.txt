add_library(VolumeApplet_Types INTERFACE)
add_library(VolumeApplet::Types ALIAS VolumeApplet_Types)

target_include_directories(VolumeApplet_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(VolumeApplet_Types INTERFACE
        Core::Applets::Types # type & Traits
        Qt${QT_VERSION_MAJOR}::Core)
