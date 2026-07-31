add_applet_library(Core_Config_Resolve_Helpers
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Types
                       TomlQt
                       tomlplusplus::tomlplusplus)
