add_subdirectory(Config)
add_subdirectory(Mapper)
add_subdirectory(Properties)

add_library(Core_Config_ConfigFile INTERFACE)
add_library(Core::Config::ConfigFile ALIAS Core_Config_ConfigFile)

target_include_directories(Core_Config_ConfigFile INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_ConfigFile INTERFACE
        Core::Config::ConfigFile::Config
        Core::Config::ConfigFile::Mapper
        Core::Config::ConfigFile::Properties)
