add_subdirectories(PathContext)

add_applet_library(Core_Config_Select
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::Log
                       Core::Config::Types
                       Core::Config::Types::Candidates
                       TomlQt
                       tomlplusplus::tomlplusplus
                       Qt${QT_VERSION_MAJOR}::Core)
