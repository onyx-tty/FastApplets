add_subdirectory(PathContext)
add_subdirectory(Types)

add_library(Core_Config_Resolve INTERFACE)
add_library(Core::Config::Resolve ALIAS Core_Config_Resolve)

target_include_directories(Core_Config_Resolve INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Resolve INTERFACE
        Core::Config::Resolve::PathContext
        Core::Config::Resolve::Types
        Core::Config::Types
        TomlQt
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
