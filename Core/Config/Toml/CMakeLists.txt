add_applet_library(Core_Config_Toml
        SOURCES Parse.cpp

        PRIVATE_DEPS Qt${QT_VERSION_MAJOR}::Core

        PUBLIC_DEPS tomlplusplus::tomlplusplus)
