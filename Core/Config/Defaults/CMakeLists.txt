add_subdirectory(Helpers)

add_applet_library(Core_Config_Defaults
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Defaults::Helpers
                       Core::Config::Types
                       Core::Config::KeysFile::Types
                       Core::Config::Types
                       Core::Applets::Types
                       Qt${QT_VERSION_MAJOR}::Widgets)
