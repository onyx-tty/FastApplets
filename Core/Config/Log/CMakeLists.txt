add_applet_library(Core_Config_Log
        SOURCES Log.cpp

        PUBLIC_DEPS Core::Config::Types::Candidates
                    TomlQt
                    tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Core)
