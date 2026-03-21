add_library(Config_Keys_Keybindings
        Keybindings.h)
add_library(Config::Keys::Keybindings ALIAS Config_Keys_Keybindings)

target_include_directories(Config_Keys_Keybindings PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})
