add_library(Config_ConfigFile_Mapper
        ConfigMapper.cpp)
add_library(Config::ConfigFile::Mapper ALIAS Config_ConfigFile_Mapper)

target_include_directories(Config_ConfigFile_Mapper PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_ConfigFile_Mapper PRIVATE
        Config::ConfigFile::Properties
        Config::Resolver
        Config::Resolver::PathContext
        Config::Resolver::Types
        Config::Types
        UI::Widgets
        UI::Types
        AppletTypes)

target_link_libraries(Config_ConfigFile_Mapper PUBLIC
        CppUtils::Log::Qt
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
