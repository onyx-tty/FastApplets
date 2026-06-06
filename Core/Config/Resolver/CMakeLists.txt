add_subdirectory(PathContext)
add_subdirectory(Types)

add_library(Core_Config_Resolver INTERFACE)
add_library(Core::Config::Resolver ALIAS Core_Config_Resolver)

target_include_directories(Core_Config_Resolver INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Resolver INTERFACE
        Core::Config::Resolver::PathContext
        Core::Config::Resolver::Types
        Core::Config::Types
        TomlQt
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
