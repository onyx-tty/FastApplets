add_applet_library(Core_Config_Validate
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Types
                       TomlQt
                       tomlplusplus::tomlplusplus
                       Qt${QT_VERSION_MAJOR}::Core)
