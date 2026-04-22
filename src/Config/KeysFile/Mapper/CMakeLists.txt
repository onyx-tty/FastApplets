add_library(Config_KeysFile_Mapper
        KeysMapper.cpp)
add_library(Config::KeysFile::Mapper ALIAS Config_KeysFile_Mapper)

target_include_directories(Config_KeysFile_Mapper PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

# TODO Swap the clauses
target_link_libraries(Config_KeysFile_Mapper PRIVATE
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Config_KeysFile_Mapper PUBLIC
        Config::KeysFile::Types
        Config::Resolvers # makeCfgPath and extendCfgPath
        Config::TOML
        Log)

