add_applet_library(Core_Config_KeysFile_Schema
        SOURCES Keys.cpp

        PRIVATE_DEPS Core::Config::KeysFile::Types
                     Core::Applets::Types)
