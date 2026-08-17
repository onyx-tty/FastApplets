add_applet_library(Core_UI_Types
        SOURCES ButtonType.cpp PerPrimaryButtonParams.cpp

        PUBLIC_DEPS Qt${QT_VERSION_MAJOR}::Widgets

        PRIVATE_DEPS Resources::Icons
                     tomlplusplus::tomlplusplus)
