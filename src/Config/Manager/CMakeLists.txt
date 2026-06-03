add_library(Config_ConfigManager INTERFACE)
add_library(Config::ConfigManager ALIAS Config_ConfigManager)

target_include_directories(Config_ConfigManager INTERFACE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_ConfigManager INTERFACE
        Config::ConfigFile::Mapper
        Config::ConfigFile::Properties # TODO: Remove after moving xFor() helpers
        Config::KeysFile::Mapper
        Config::FileLocator
        Config::TomlParser
        Config::Types
        AppletTypes)

target_link_libraries(Config_ConfigManager INTERFACE
        Qt${QT_VERSION_MAJOR}::Widgets)
