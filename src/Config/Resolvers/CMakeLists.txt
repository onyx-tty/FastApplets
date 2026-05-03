add_library(Config_Resolvers
        Resolvers.cpp)
add_library(Config::Resolvers ALIAS Config_Resolvers)

target_include_directories(Config_Resolvers PRIVATE
        ${SRC_DIR})

target_link_libraries(Config_Resolvers PRIVATE
        Config::TOML)

target_link_libraries(Config_Resolvers PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
