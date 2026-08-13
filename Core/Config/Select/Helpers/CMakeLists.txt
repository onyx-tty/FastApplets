add_applet_library(Core_Config_Select_Helpers
        SOURCES Helpers.cpp

        PRIVATE_DEPS Core::Config::Select::Types

        PUBLIC_DEPS TomlQt
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Core)
