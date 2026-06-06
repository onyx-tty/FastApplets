add_library(Core_Config_ConfigFile_PowerApplet
        PowerAppletConfig.cpp)
add_library(Core::Config::ConfigFile::PowerApplet ALIAS Core_Config_ConfigFile_PowerApplet)

target_include_directories(Core_Config_ConfigFile_PowerApplet PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# TODO: This should explicitly get PowerButtonParams which is a part of
#       UI instead of Core::UI::Types. Move PowerButtonParams to Core::UI::Types.
target_link_libraries(Core_Config_ConfigFile_PowerApplet PRIVATE
        Core::Config::ConfigFile::Mapper
        Core::Config::ConfigFile::Properties
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Core_Config_ConfigFile_PowerApplet PUBLIC
        Core::Config::ConfigFile::Global
        Core::UI::Widgets)
