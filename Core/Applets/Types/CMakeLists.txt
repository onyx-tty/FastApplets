add_library(Core_Applets_Types INTERFACE)
add_library(Core::Applets::Types ALIAS Core_Applets_Types)

target_include_directories(Core_Applets_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
