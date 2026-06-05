add_library(Core_Config_KeysFile_PowerApplet
        PowerAppletKeys.cpp)
add_library(Core::Config::KeysFile::PowerApplet ALIAS Core_Config_KeysFile_PowerApplet)

target_include_directories(Core_Config_KeysFile_PowerApplet PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_KeysFile_PowerApplet PRIVATE
        Core::Config::KeysFile::Global
        Core::Config::KeysFile::Mapper
        Core::Config::KeysFile::Types)

target_link_libraries(Core_Config_KeysFile_PowerApplet PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
