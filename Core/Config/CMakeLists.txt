# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Requirements
find_package(tomlplusplus REQUIRED)

add_subdirectories(AppletConfig ConfigFile KeysFile Defaults Locate Resolve Toml Types)

add_applet_library(Core_Config INTERFACE
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::ConfigFile
                       Core::Config::KeysFile
                       Core::Config::AppletConfig
                       Core::Config::Defaults
                       Core::Config::Locate
                       Core::Config::Toml
                       Core::Config::Resolve
                       Core::Config::Types
                       Core::UI::Widgets
                       CppUtils::String
                       tomlplusplus::tomlplusplus
                       Qt${QT_VERSION_MAJOR}::Widgets)
