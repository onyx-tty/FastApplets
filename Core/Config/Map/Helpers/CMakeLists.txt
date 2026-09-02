add_applet_library(Core_Config_Map_Helpers
        SOURCES Config.cpp Keys.cpp

        PUBLIC_DEPS Core::Config::View
                    Core::Config::Types
                    Core::Shell::Types
                    Core::UI::Types
                    Core::Applets::Types
                    TomlQt
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Core

        PRIVATE_DEPS Qt${QT_VERSION_MAJOR}::Widgets)
