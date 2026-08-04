add_subdirectories(Helpers PathContext Types)

add_applet_library(Core_Config_Resolve
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Resolve::Helpers
                       Core::Config::Resolve::PathContext
                       Core::Config::Resolve::Types
                       TomlQt
                       Qt${QT_VERSION_MAJOR}::Core
                       tomlplusplus::tomlplusplus)
