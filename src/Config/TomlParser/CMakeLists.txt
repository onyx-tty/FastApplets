add_library(Config_TomlParser
        TomlParser.cpp)
add_library(Config::TomlParser ALIAS Config_TomlParser)

target_include_directories(Config_TomlParser PRIVATE
        ${SRC_DIR})

target_link_libraries(Config_TomlParser PRIVATE
        Config::FileLocator
        Config::Types)

target_link_libraries(Config_TomlParser PUBLIC
        CppUtils::Log::Qt
        tomlplusplus::tomlplusplus)
