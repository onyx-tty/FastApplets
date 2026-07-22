add_applet_library(Core_Args
        SOURCES Args.cpp

        PRIVATE_DEPS Qt${QT_VERSION_MAJOR}::Widgets

        PUBLIC_DEPS Core::Applets::Types
                    Core::Config::Types) # Only needed for injectArgs
