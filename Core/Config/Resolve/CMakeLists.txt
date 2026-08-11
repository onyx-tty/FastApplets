add_applet_library(Core_Config_Resolve
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Select
                       Core::Config::Select::Types
                       TomlQt
                       Qt${QT_VERSION_MAJOR}::Core
                       tomlplusplus::tomlplusplus)
