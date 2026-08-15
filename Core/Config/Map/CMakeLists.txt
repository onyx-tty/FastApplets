add_subdirectory(Helpers)

add_applet_library(Core_Config_Map
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Map::Helpers
                       Core::Config::Schema
                       Core::Config::Resolve
                       Core::Config::Log
                       Core::Config::Types
                       Core::Config::Types::Candidates
                       Core::Config::Types::PathContext
                       Core::UI::Types
                       Core::Applets::Types
                       tomlplusplus::tomlplusplus
                       Qt${QT_VERSION_MAJOR}::Widgets)
