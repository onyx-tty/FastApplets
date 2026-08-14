add_applet_library(Core_Config_AppletConfig
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Schema
                       Core::Config::Map
                       Core::Config::Defaults
                       Core::Config::Toml
                       Core::Config::Types
                       Core::Applets::Types
                       Qt${QT_VERSION_MAJOR}::Widgets)
