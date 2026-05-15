add_library(Config_ConfigFile_Global
        GlobalConfig.cpp)
add_library(Config::ConfigFile::Global ALIAS Config_ConfigFile_Global)

target_include_directories(Config_ConfigFile_Global PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_ConfigFile_Global PRIVATE
        Config::ConfigFile::Properties)
