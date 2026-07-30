add_subdirectories(Schema Map Types)

add_applet_library(Core_Config_KeysFile
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::KeysFile::Schema
                       Core::Config::KeysFile::Map
                       Core::Config::KeysFile::Types)
