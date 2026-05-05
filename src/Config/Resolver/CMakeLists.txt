add_subdirectory(PathContext)
add_subdirectory(Types)

add_library(Config_Resolver INTERFACE)
add_library(Config::Resolver ALIAS Config_Resolver)

target_include_directories(Config_Resolver INTERFACE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Resolver INTERFACE
        Config::Resolver::PathContext
        Config::Resolver::Types
        Config::TomlParser
        Config::Types)

target_link_libraries(Config_Resolver INTERFACE
        TomlQt
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
