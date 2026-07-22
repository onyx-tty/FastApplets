add_applet_library(Core_Config_Locate
        SOURCES Locate.cpp

        PRIVATE_DEPS Core::Config::Types

        PUBLIC_DEPS Qt${QT_VERSION_MAJOR}::Core)
