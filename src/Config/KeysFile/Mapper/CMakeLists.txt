add_library(Config_KeysFile_Mapper
        KeysMapper.cpp)
add_library(Config::KeysFile::Mapper ALIAS Config_KeysFile_Mapper)

target_include_directories(Config_KeysFile_Mapper PRIVATE
        ${SRC_DIR})

target_link_libraries(Config_KeysFile_Mapper PRIVATE
        Config::KeysFile::Types
        Config::Resolvers # PathContext
        Config::TOML
        TomlQt) # TODO This module doesn't use TomlQt but the Resolver & Mapper do
                # so this file has to link against it too. Maybe Resolver should be public?

target_link_libraries(Config_KeysFile_Mapper PUBLIC
        CppUtils::Log::Qt
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
