add_subdirectory(Global)
add_subdirectory(Mapper)
add_subdirectory(PowerApplet)
add_subdirectory(Properties)

add_library(Config_ConfigFile INTERFACE)
add_library(Config::ConfigFile ALIAS Config_ConfigFile)

target_include_directories(Config_ConfigFile INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_ConfigFile INTERFACE
        Config::ConfigFile::Global
        Config::ConfigFile::Mapper
        Config::ConfigFile::PowerApplet
        Config::ConfigFile::Properties
        Config::TOML
        UI::Enums
        UI::Widgets
        CppUtils::Log::Qt)
