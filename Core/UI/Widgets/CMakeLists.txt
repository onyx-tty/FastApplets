add_applet_library(Core_UI_Widgets
        SOURCES PrimaryButton.cpp

        PRIVATE_DEPS Core::Config::KeysFile::Types
                     Core::UI::Types
                     Core::Shell

        PUBLIC_DEPS Qt${QT_VERSION_MAJOR}::Widgets)
