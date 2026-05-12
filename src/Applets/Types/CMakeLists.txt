add_library(AppletTypes
        AppletType.h
        AppletRecord.h)

target_include_directories(AppletTypes PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
