add_library(Config_ConfigFile_Mapper
        ConfigMapper.cpp)
add_library(Config::ConfigFile::Mapper ALIAS Config_ConfigFile_Mapper)

target_include_directories(Config_ConfigFile_Mapper PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_ConfigFile_Mapper PRIVATE
        Config::ConfigFile::Properties
        Config::TOML
        UI::Enums
        UI::Widgets)

target_link_libraries(Config_ConfigFile_Mapper PUBLIC
        CppUtils::Log
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
