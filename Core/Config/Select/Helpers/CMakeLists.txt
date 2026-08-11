add_applet_library(Core_Config_Select_Helpers
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Select::Types
                       TomlQt
                       tomlplusplus::tomlplusplus
                       Qt${QT_VERSION_MAJOR}::Core)
