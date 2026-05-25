add_library(Config_Resolver_PathContext
        PathContext.cpp)
add_library(Config::Resolver::PathContext ALIAS Config_Resolver_PathContext)

target_include_directories(Config_Resolver_PathContext PRIVATE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Resolver_PathContext PRIVATE
        AppletTypes)

target_link_libraries(Config_Resolver_PathContext PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
