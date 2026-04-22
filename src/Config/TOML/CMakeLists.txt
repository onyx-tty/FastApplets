add_subdirectory(Types)

add_library(Config_TOML
        TomlAccessor.cpp
        TomlLocator.cpp
        TomlParser.cpp)
add_library(Config::TOML ALIAS Config_TOML)

target_include_directories(Config_TOML PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_TOML PRIVATE
        Config::TOML::Types
        Log)

target_link_libraries(Config_TOML PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus
        CppUtils)
