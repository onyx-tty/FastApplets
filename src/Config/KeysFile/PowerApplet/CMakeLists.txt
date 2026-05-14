add_library(Config_KeysFile_PowerApplet
        PowerAppletKeys.cpp)
add_library(Config::KeysFile::PowerApplet ALIAS Config_KeysFile_PowerApplet)

target_include_directories(Config_KeysFile_PowerApplet PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_KeysFile_PowerApplet PRIVATE
        Config::KeysFile::Mapper
        Config::KeysFile::Types
        Config::FileLocator
        Config::TomlParser
        AppletTypes
        TomlQt) # TODO This module doesn't use TomlQt but the Resolver & Mapper do
                # so this file has to link against it too. Maybe Resolver should be public?
