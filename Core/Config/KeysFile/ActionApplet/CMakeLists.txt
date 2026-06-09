add_library(Core_Config_KeysFile_ActionApplet
        ActionAppletKeys.cpp)
add_library(Core::Config::KeysFile::ActionApplet ALIAS Core_Config_KeysFile_ActionApplet)

target_include_directories(Core_Config_KeysFile_ActionApplet PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_KeysFile_ActionApplet PRIVATE
        Core::Config::KeysFile::Mapper
        Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(Core_Config_KeysFile_ActionApplet PUBLIC
        Core::Config::KeysFile::Global
        Core::Config::KeysFile::Types)
