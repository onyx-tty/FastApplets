add_applet_library(Core_Config_Log
        SOURCES Log.cpp

        PRIVATE_DEPS Core::Config::Types::Candidates

        PUBLIC_DEPS TomlQt
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Core)
