add_library(Core_Config_KeysFile_Keys
        Keys.cpp)
add_library(Core::Config::KeysFile::Keys ALIAS Core_Config_KeysFile_Keys)

target_include_directories(Core_Config_KeysFile_Keys PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_KeysFile_Keys PRIVATE
        Core::Config::KeysFile::Types)
