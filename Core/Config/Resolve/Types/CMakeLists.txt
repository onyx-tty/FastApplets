add_applet_library(Core_Config_Resolve_Types
        SOURCES Candidate.cpp Candidates.cpp

        PUBLIC_DEPS Core::Config::Types
                    Core::Applets::Types
                    Qt${QT_VERSION_MAJOR}::Core)
