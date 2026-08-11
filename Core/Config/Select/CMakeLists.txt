add_subdirectories(Helpers PathContext Types)

add_applet_library(Core_Config_Select
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Select::Helpers
                       Core::Config::Select::Types
                       Core::Config::Types
                       TomlQt
                       tomlplusplus::tomlplusplus
                       Qt${QT_VERSION_MAJOR}::Core)
