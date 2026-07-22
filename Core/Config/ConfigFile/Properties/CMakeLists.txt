add_applet_library(Core_Config_ConfigFile_Properties
        SOURCES Window.cpp PrimaryButton.cpp Layout.cpp

        PRIVATE_DEPS Core::UI::Types
                     Resources::Icons

        PUBLIC_DEPS tomlplusplus::tomlplusplus
                    Qt${QT_VERSION_MAJOR}::Widgets)
