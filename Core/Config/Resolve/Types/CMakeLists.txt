add_applet_library(Core_Config_Resolve_Types
        SOURCES ResolverCandidate.cpp

        PRIVATE_DEPS Qt${QT_VERSION_MAJOR}::Core

        PUBLIC_DEPS Core::Config::Types
                    Core::Applets::Types)
