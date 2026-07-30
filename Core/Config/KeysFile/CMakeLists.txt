add_subdirectories(Schema Mapper Types)

add_applet_library(Core_Config_KeysFile
        TYPE INTERFACE

        INTERFACE_DEPS Core::Config::KeysFile::Schema
                       Core::Config::KeysFile::Mapper
                       Core::Config::KeysFile::Types)
