add_library(Core_Config_KeysFile_Global
        GlobalKeys.cpp)
add_library(Core::Config::KeysFile::Global ALIAS Core_Config_KeysFile_Global)

target_include_directories(Core_Config_KeysFile_Global PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_KeysFile_Global PRIVATE
        Core::Config::KeysFile::Types)
