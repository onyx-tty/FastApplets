add_applet_library(Core_UI_Widgets
        SOURCES PrimaryButton.cpp

        PRIVATE_DEPS Core::UI::Types
                     Core::Config::Types
                     Core::Shell

        PUBLIC_DEPS Qt${QT_VERSION_MAJOR}::Widgets)
