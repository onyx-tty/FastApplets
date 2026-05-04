add_library(UI_Enums
        ButtonIDs.h)
add_library(UI::Enums ALIAS UI_Enums)

target_include_directories(UI_Enums PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
