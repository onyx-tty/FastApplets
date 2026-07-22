add_applet_library(Core_Shell
        SOURCES Shell.cpp

        PUBLIC_DEPS Qt${QT_VERSION_MAJOR}::Core)
