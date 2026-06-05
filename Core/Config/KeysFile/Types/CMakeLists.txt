add_library(Core_Config_KeysFile_Types INTERFACE)
add_library(Core::Config::KeysFile::Types ALIAS Core_Config_KeysFile_Types)

target_include_directories(Core_Config_KeysFile_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
