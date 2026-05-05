add_library(Config_Resolver
        Resolver.cpp)
add_library(Config::Resolver ALIAS Config_Resolver)

target_include_directories(Config_Resolver PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Resolver PRIVATE
        Config::TomlParser
        Config::Types)

target_link_libraries(Config_Resolver PUBLIC
        TomlQt
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
