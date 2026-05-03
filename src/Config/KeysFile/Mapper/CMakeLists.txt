add_library(Config_KeysFile_Mapper
        KeysMapper.cpp)
add_library(Config::KeysFile::Mapper ALIAS Config_KeysFile_Mapper)

target_include_directories(Config_KeysFile_Mapper PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_KeysFile_Mapper PRIVATE
        Config::KeysFile::Types
        Config::Resolvers # PathContext
        Config::TOML)

target_link_libraries(Config_KeysFile_Mapper PUBLIC
        CppUtils::Log::Qt
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
