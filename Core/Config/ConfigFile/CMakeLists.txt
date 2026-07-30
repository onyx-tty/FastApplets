add_subdirectories(Schema Map)

add_applet_library(Core_Config_ConfigFile
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::ConfigFile::Schema
                       Core::Config::ConfigFile::Map)
