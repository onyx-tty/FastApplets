add_library(Core_Config_Resolver_PathContext
        PathContext.cpp)
add_library(Core::Config::Resolver::PathContext ALIAS Core_Config_Resolver_PathContext)

target_include_directories(Core_Config_Resolver_PathContext PRIVATE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Resolver_PathContext PUBLIC
        Core::Applets::Types
        Qt${QT_VERSION_MAJOR}::Core)
