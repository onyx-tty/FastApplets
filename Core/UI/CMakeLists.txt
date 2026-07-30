add_subdirectories(Widgets Types)

add_applet_library(Core_UI
        SOURCES CentralWidget.cpp
                MainWindow.cpp

        PRIVATE_DEPS Core::Config::ConfigFile::Schema
                     Core::Config::KeysFile::Schema

        PUBLIC_DEPS Core::UI::Widgets
                    Core::UI::Types
                    Core::Config::KeysFile::Types
                    Core::Applets::Types
                    Qt${QT_VERSION_MAJOR}::Widgets)
