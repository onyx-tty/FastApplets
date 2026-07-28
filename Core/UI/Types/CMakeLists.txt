add_applet_library(Core_UI_Types
        SOURCES ButtonType.cpp LayoutProperties.cpp

        PUBLIC_DEPS Core::UI::Types
                    Qt${QT_VERSION_MAJOR}::Widgets

        PRIVATE_DEPS Core::UI::Types
                     Resources::Icons
                     tomlplusplus::tomlplusplus)
