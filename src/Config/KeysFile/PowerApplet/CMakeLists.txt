add_library(Config_KeysFile_PowerApplet
        PowerAppletKeys.cpp)
add_library(Config::KeysFile::PowerApplet ALIAS Config_KeysFile_PowerApplet)

target_include_directories(Config_KeysFile_PowerApplet PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_KeysFile_PowerApplet PRIVATE
        Config::KeysFile::Global
        Config::KeysFile::Mapper
        Config::KeysFile::Types
        Config::FileLocator
        Config::TomlParser
        Config::Types
        AppletTypes)

target_link_libraries(Config_KeysFile_PowerApplet PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
