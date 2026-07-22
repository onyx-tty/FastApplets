add_subdirectory(Config)
add_subdirectory(Mapper)
add_subdirectory(Properties)

add_applet_library(Core_Config_ConfigFile
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::ConfigFile::Config
                       Core::Config::ConfigFile::Mapper
                       Core::Config::ConfigFile::Properties)
