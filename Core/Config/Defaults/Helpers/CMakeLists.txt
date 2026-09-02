add_applet_library(Core_Config_Defaults_Helpers
        TYPE INTERFACE

        INTERFACE_DEPS Core::Shell::Types
                       Core::UI::Types
                       Core::Applets::Types
                       Qt${QT_VERSION_MAJOR}::Widgets)
