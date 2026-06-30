add_library(Core_Config_KeysFile_Mapper
        KeysMapper.cpp)
add_library(Core::Config::KeysFile::Mapper ALIAS Core_Config_KeysFile_Mapper)

target_include_directories(Core_Config_KeysFile_Mapper PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_KeysFile_Mapper PRIVATE
        Core::Config::TomlParser
        Core::Config::Resolver)

target_link_libraries(Core_Config_KeysFile_Mapper PUBLIC
        Core::Config::KeysFile::Keys
        Core::Config::KeysFile::Types
        Core::Config::Resolver::PathContext
        Core::Config::Resolver::Types
        Core::Config::Types
        Core::Applets::Types
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
