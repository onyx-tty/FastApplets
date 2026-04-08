add_library(Config_KeysFile_Types
        Keybindings.h)
add_library(Config::KeysFile::Types ALIAS Config_KeysFile_Types)

target_include_directories(Config_KeysFile_Types PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})
