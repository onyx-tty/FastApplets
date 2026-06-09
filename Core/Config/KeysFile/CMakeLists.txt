add_subdirectory(Global)
add_subdirectory(Mapper)
add_subdirectory(PowerApplet)
add_subdirectory(ActionApplet)
add_subdirectory(Types)

add_library(Core_Config_KeysFile INTERFACE)
add_library(Core::Config::KeysFile ALIAS Core_Config_KeysFile)

target_include_directories(Core_Config_KeysFile INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_KeysFile INTERFACE
        Core::Config::KeysFile::Global
        Core::Config::KeysFile::Mapper
        Core::Config::KeysFile::PowerApplet
        Core::Config::KeysFile::Types)
