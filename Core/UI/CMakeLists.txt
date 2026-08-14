add_subdirectories(Widgets Types)

add_applet_library(Core_UI
        SOURCES CentralWidget.cpp
                MainWindow.cpp

        PRIVATE_DEPS Core::Config::Schema

        PUBLIC_DEPS Core::Config::Types
                    Core::UI::Widgets
                    Core::UI::Types
                    Core::Applets::Types
                    Qt${QT_VERSION_MAJOR}::Widgets)
