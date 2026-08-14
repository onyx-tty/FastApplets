add_applet_library(Core_Config_ConfigFile_Map
        SOURCES Map.cpp

        PUBLIC_DEPS Core::Config::ConfigFile::Schema
                    Core::Config::Resolve
                    Core::Config::Select::PathContext
                    Core::Config::Types
                    Core::Config::Types::Candidates
                    Core::UI::Types
                    Core::Applets::Types
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Widgets)
