add_library(Config_ConfigManager
        ConfigManager.cpp)
add_library(Config::ConfigManager ALIAS Config_ConfigManager)

target_include_directories(Config_ConfigManager PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_ConfigManager PRIVATE
        Config::ConfigFile::PowerApplet
        Config::ConfigFile::Mapper
        Config::KeysFile::PowerApplet
        Config::KeysFile::Mapper
        Config::FileLocator
        Config::TomlParser
        Config::Types
        AppletTypes)

target_link_libraries(Config_ConfigManager PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
