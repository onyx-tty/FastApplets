add_applet_library(Core_Config_Types_PathContext
        SOURCES PathContext.cpp

        PUBLIC_DEPS Core::Applets::Types
                    Qt${QT_VERSION_MAJOR}::Core)
