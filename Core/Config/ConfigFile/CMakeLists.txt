add_subdirectories(Config Mapper Properties)

add_applet_library(Core_Config_ConfigFile
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::ConfigFile::Config
                       Core::Config::ConfigFile::Mapper
                       Core::Config::ConfigFile::Properties)
