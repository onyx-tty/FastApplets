add_library(Core_Config_Resolver_Types
        ResolverCandidate.cpp)
add_library(Core::Config::Resolver::Types ALIAS Core_Config_Resolver_Types)

target_include_directories(Core_Config_Resolver_Types PRIVATE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Resolver_Types PRIVATE
        Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(Core_Config_Resolver_Types PUBLIC
        Core::Config::Types
        Core::Applets::Types)
