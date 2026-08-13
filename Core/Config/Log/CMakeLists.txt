add_applet_library(Core_Config_Log
        SOURCES Log.cpp

        PRIVATE_DEPS Core::Config::Select::Types

        PUBLIC_DEPS TomlQt
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Core)
