add_library(Config_KeysFile_Mapper
        KeysMapper.cpp)
add_library(Config::KeysFile::Mapper ALIAS Config_KeysFile_Mapper)

target_include_directories(Config_KeysFile_Mapper PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_KeysFile_Mapper PRIVATE
        Config::KeysFile::Types
        Config::TomlParser
        Config::Types
        AppletTypes)

target_link_libraries(Config_KeysFile_Mapper PUBLIC
        Config::Resolver
        Config::Resolver::PathContext
        Config::Resolver::Types
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
