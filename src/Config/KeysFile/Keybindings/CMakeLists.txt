add_library(Config_KeysFile_Keybindings
        Keybindings.h)
add_library(Config::KeysFile::Keybindings ALIAS Config_KeysFile_Keybindings)

target_include_directories(Config_KeysFile_Keybindings PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})
