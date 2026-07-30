add_applet_library(Core_Config_KeysFile_Map
        SOURCES Map.cpp

        PRIVATE_DEPS Core::Config::Toml
                     Core::Config::Resolve

        PUBLIC_DEPS Core::Config::KeysFile::Schema
                    Core::Config::KeysFile::Types
                    Core::Config::Resolve::PathContext
                    Core::Config::Resolve::Types
                    Core::Config::Types
                    Core::Applets::Types
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Widgets)
