add_library(Core_Config_ConfigFile_Config
        Config.cpp)
add_library(Core::Config::ConfigFile::Config ALIAS Core_Config_ConfigFile_Config)

target_include_directories(Core_Config_ConfigFile_Config PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_ConfigFile_Config PUBLIC
        Core::Config::ConfigFile::Properties)
