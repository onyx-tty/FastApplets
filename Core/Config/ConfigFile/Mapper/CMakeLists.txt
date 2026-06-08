add_library(Core_Config_ConfigFile_Mapper
        ConfigMapper.cpp)
add_library(Core::Config::ConfigFile::Mapper ALIAS Core_Config_ConfigFile_Mapper)

target_include_directories(Core_Config_ConfigFile_Mapper PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_ConfigFile_Mapper PRIVATE
        Core::UI::Widgets
        Core::UI::Types) # TODO: Only for PowerButtonParams, move it elsewhere

target_link_libraries(Core_Config_ConfigFile_Mapper PUBLIC
        Core::Config::ConfigFile::Properties
        Core::Config::Resolver
        Core::Config::Resolver::PathContext
        Core::Config::Resolver::Types
        Core::Applets::Types
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
