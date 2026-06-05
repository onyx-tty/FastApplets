add_library(Core_AppletTypes INTERFACE)
add_library(Core::AppletTypes ALIAS Core_AppletTypes)

target_include_directories(Core_AppletTypes INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
