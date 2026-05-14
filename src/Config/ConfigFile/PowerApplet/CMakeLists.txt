add_library(Config_ConfigFile_PowerApplet
        PowerAppletConfig.cpp)
add_library(Config::ConfigFile::PowerApplet ALIAS Config_ConfigFile_PowerApplet)

target_include_directories(Config_ConfigFile_PowerApplet PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# TODO: This should explicitly get PowerButtonParams which is a part of
#       UI instead of UI::Types. Move PowerButtonParams to UI::Types.
target_link_libraries(Config_ConfigFile_PowerApplet PRIVATE
        Config::ConfigFile::Global
        Config::ConfigFile::Mapper
        Config::ConfigFile::Properties
        Config::FileLocator
        Config::TomlParser
        AppletTypes
        UI::Types
        Shell)

target_link_libraries(Config_ConfigFile_PowerApplet PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
