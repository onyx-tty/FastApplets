add_library(Config_Resolver_Types INTERFACE)
add_library(Config::Resolver::Types ALIAS Config_Resolver_Types)

target_include_directories(Config_Resolver_Types INTERFACE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Resolver_Types INTERFACE
        Config::Types)

target_link_libraries(Config_Resolver_Types INTERFACE
        tomlplusplus::tomlplusplus)
