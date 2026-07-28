add_applet_library(Core_Config_Defaults
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Types
                       Core::Config::ConfigFile::Properties
                       Core::Config::KeysFile::Types
                       Core::Config::Types
                       Core::UI::Types
                       Core::Applets::Types
                       Qt${QT_VERSION_MAJOR}::Widgets)
