add_library(Config_KeysFile_Global
        GlobalKeys.cpp)
add_library(Config::KeysFile::Global ALIAS Config_KeysFile_Global)

target_include_directories(Config_KeysFile_Global PRIVATE
        ${SRC_DIR})

target_link_libraries(Config_KeysFile_Global PRIVATE
        Config::KeysFile::Types)

target_link_libraries(Config_KeysFile_Global PUBLIC
        CppUtils::Log::Qt)
