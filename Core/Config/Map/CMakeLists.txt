add_applet_library(Core_Config_Map
        SOURCES Map.cpp

        PUBLIC_DEPS Core::Config::Schema
                    Core::Config::Resolve
                    Core::Config::Log
                    Core::Config::Types
                    Core::Config::Types::Candidates
                    Core::Config::Types::PathContext
                    Core::UI::Types
                    Core::Applets::Types
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Widgets)
