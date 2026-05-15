add_library(AppletTypes INTERFACE)

target_include_directories(AppletTypes INTERFACE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
