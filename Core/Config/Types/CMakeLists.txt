add_subdirectories(Candidates PathContext)

add_applet_library(Core_Config_Types
        TYPE INTERFACE

        INTERFACE_DEPS tomlplusplus::tomlplusplus
                       Qt${QT_VERSION_MAJOR}::Core)
