add_subdirectory(Global)
add_subdirectory(PowerApplet)
add_subdirectory(Properties)

add_library(Config_ConfigFile
        ConfigMapper.cpp)
add_library(Config::ConfigFile ALIAS Config_ConfigFile)

target_include_directories(Config_ConfigFile PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_ConfigFile PRIVATE
        Config::ConfigFile::Global
        Config::ConfigFile::PowerApplet
        Config::ConfigFile::Properties
        Config::TOML
        Log
        UI::Enums
        UI::Widgets)

target_link_libraries(Config_ConfigFile PUBLIC
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
