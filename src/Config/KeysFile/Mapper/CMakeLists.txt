add_library(Config_KeysFile_Mapper
        KeysMapper.cpp)
add_library(Config::KeysFile::Mapper ALIAS Config_KeysFile_Mapper)

target_include_directories(Config_KeysFile_Mapper PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_KeysFile_Mapper PRIVATE
        Config::KeysFile::Types
        Config::Resolver::PathContext
        Config::Resolver::Types
        Config::TomlParser
        TomlQt) # TODO This module doesn't use TomlQt but the Resolver & Mapper do
                # so this file has to link against it too. Maybe Resolver should be public?

target_link_libraries(Config_KeysFile_Mapper PUBLIC
        CppUtils::Log::Qt
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
