add_library(Config_Types INTERFACE)
add_library(Config::Types ALIAS Config_Types)

target_include_directories(Config_Types INTERFACE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Types INTERFACE
        tomlplusplus::tomlplusplus)
