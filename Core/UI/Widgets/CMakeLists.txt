add_applet_library(Core_UI_Widgets
        SOURCES PrimaryButton.cpp

        PRIVATE_DEPS Core::Config::ConfigFile::Properties
                     Core::Config::KeysFile::Types
                     Core::Shell

        PUBLIC_DEPS Core::UI::Types # PrimaryButtonParams
                    Qt${QT_VERSION_MAJOR}::Widgets)
