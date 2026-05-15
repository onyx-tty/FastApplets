add_library(UI_Types INTERFACE)
add_library(UI::Types ALIAS UI_Types)

target_include_directories(UI_Types INTERFACE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
