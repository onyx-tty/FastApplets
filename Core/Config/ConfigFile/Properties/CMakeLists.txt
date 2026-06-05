add_library(Core_Config_ConfigFile_Properties
        WindowProperties.cpp
        PrimaryButtonProperties.cpp
        LayoutProperties.cpp)
add_library(Core::Config::ConfigFile::Properties ALIAS Core_Config_ConfigFile_Properties)

target_include_directories(Core_Config_ConfigFile_Properties PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_ConfigFile_Properties PRIVATE
        Core::UI::Types
        Resources::Icons)

target_link_libraries(Core_Config_ConfigFile_Properties PUBLIC
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
