add_library(UI_Enums
        ButtonIDs.h)
add_library(UI::Enums ALIAS UI_Enums)

target_include_directories(UI_Enums PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${SRC_DIR})
