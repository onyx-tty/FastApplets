add_library(Core_Config_ConfigManager INTERFACE)
add_library(Core::Config::ConfigManager ALIAS Core_Config_ConfigManager)

target_include_directories(Core_Config_ConfigManager INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_ConfigManager INTERFACE
        Core::Config::ConfigFile::Config
        Core::Config::ConfigFile::Mapper
        Core::Config::KeysFile::Keys
        Core::Config::KeysFile::Mapper
        Core::Config::Factory
        Core::Config::FileLocator
        Core::Config::TomlParser
        Core::Config::Types
        Core::Applets::Types
        Qt${QT_VERSION_MAJOR}::Widgets)
