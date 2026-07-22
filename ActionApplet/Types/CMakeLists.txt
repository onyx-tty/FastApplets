add_applet_library(ActionApplet_Types
        TYPE INTERFACE

        INTERFACE_DEPS Core::UI::Types # button_type
                       Core::Applets::Types # type & Traits
                       Qt${QT_VERSION_MAJOR}::Core)
