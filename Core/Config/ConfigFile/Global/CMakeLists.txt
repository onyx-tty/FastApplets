add_library(Core_Config_ConfigFile_Global
        GlobalConfig.cpp)
add_library(Core::Config::ConfigFile::Global ALIAS Core_Config_ConfigFile_Global)

target_include_directories(Core_Config_ConfigFile_Global PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_ConfigFile_Global PUBLIC
        Core::Config::ConfigFile::Properties)
