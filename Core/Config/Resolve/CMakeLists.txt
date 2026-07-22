add_subdirectories(PathContext Types)

add_applet_library(Core_Config_Resolve
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Resolve::PathContext
                       Core::Config::Resolve::Types
                       Core::Config::Types
                       TomlQt
                       Qt${QT_VERSION_MAJOR}::Widgets
                       tomlplusplus::tomlplusplus)
