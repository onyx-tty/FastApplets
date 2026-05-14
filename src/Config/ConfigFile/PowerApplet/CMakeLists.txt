add_library(Config_ConfigFile_PowerApplet
        PowerAppletConfig.cpp)
add_library(Config::ConfigFile::PowerApplet ALIAS Config_ConfigFile_PowerApplet)

target_include_directories(Config_ConfigFile_PowerApplet PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_ConfigFile_PowerApplet PRIVATE
        Config::ConfigFile::Mapper
        Config::ConfigFile::Properties
        Config::FileLocator
        Config::TomlParser
        AppletTypes
        Shell)

target_link_libraries(Config_ConfigFile_PowerApplet PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
