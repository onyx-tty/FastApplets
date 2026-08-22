add_applet_library(Core_Config_View
        SOURCES View.cpp

        PUBLIC_DEPS Core::Config::Validate
                    Core::Config::Types
                    TomlQt
                    Qt${QT_VERSION_MAJOR}::Core

        PRIVATE_DEPS tomlplusplus::tomlplusplus)
