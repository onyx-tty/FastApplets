add_library(Config_Resolvers
        Resolvers.cpp)
add_library(Config::Resolvers ALIAS Config_Resolvers)

target_include_directories(Config_Resolvers PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Resolvers PRIVATE
        Config::TomlParser
        Config::Types)

target_link_libraries(Config_Resolvers PUBLIC
        TomlQt
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
