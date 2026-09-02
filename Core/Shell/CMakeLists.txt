add_subdirectories(Types)

add_applet_library(Core_Shell
        SOURCES Shell.cpp

        PUBLIC_DEPS Core::Shell::Types
                    Qt${QT_VERSION_MAJOR}::Core)
