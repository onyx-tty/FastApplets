add_applet_library(Core_Config_ConfigFile_Config
        SOURCES Config.cpp

        PUBLIC_DEPS Core::Config::ConfigFile::Properties
                    Core::Applets::Types)
