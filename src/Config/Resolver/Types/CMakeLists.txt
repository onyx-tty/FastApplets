add_library(Config_Resolver_Types
        ResolverCandidate.cpp)
add_library(Config::Resolver::Types ALIAS Config_Resolver_Types)

target_include_directories(Config_Resolver_Types PRIVATE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Resolver_Types PRIVATE
        Config::Types)
