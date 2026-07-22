add_applet_library(Core_Config_AppletConfig
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::ConfigFile::Config
                       Core::Config::ConfigFile::Mapper
                       Core::Config::KeysFile::Keys
                       Core::Config::KeysFile::Mapper
                       Core::Config::Defaults
                       Core::Config::Toml
                       Core::Config::Types
                       Core::Applets::Types
                       Qt${QT_VERSION_MAJOR}::Widgets)
