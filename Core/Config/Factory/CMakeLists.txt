add_library(Core_Config_Factory INTERFACE)
add_library(Core::Config::Factory ALIAS Core_Config_Factory)

target_include_directories(Core_Config_Factory INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Factory INTERFACE
        Core::Config::ConfigFile::Properties
        Core::Config::KeysFile::Types
        Core::Config::Types
        Core::UI::Types
        Core::Applets::Types
        Qt${QT_VERSION_MAJOR}::Widgets)
