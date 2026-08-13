add_applet_library(Core_Config_Select_Types
        SOURCES Candidate.cpp Candidates.cpp

        PUBLIC_DEPS Core::Config::Types
                    Core::Applets::Types
                    TomlQt
                    Qt${QT_VERSION_MAJOR}::Core)
