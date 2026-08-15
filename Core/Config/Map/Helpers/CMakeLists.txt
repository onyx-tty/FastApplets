add_applet_library(Core_Config_Map_Helpers
        SOURCES Config.cpp Keys.cpp

        PUBLIC_DEPS Core::Config::Resolve
                    Core::Config::Log
                    Core::Config::Map
                    Core::Config::Types
                    Core::Config::Types::Candidates
                    Core::UI::Types
                    Core::Applets::Types
                    TomlQt
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Widgets)
