add_subdirectories(Widgets Types)

add_applet_library(Core_UI
        SOURCES CentralWidget.cpp
                MainWindow.cpp

        PRIVATE_DEPS Core::Config::ConfigFile::Config
                     Core::Config::KeysFile::Keys

        PUBLIC_DEPS Core::UI::Widgets
                    Core::UI::Types
                    Core::Config::ConfigFile::Properties
                    Core::Config::KeysFile::Types
                    Core::Applets::Types
                    Qt${QT_VERSION_MAJOR}::Widgets)
