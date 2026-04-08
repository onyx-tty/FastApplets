add_library(Config_KeysFile_Global
        GlobalKeys.cpp)
add_library(Config::KeysFile::Global ALIAS Config_KeysFile_Global)

target_include_directories(Config_KeysFile_Global PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_KeysFile_Global PRIVATE
        Config::KeysFile::Keybindings)
