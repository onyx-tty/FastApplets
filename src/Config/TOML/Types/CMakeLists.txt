add_library(Config_TOML_Types
        ConfigTomlFiles.h
        NodePair.h
        NodeView.h
        TomlArrayConditions.h)
add_library(Config::TOML::Types ALIAS Config_TOML_Types)

target_include_directories(Config_TOML_Types PRIVATE
        ${SRC_DIR})

target_link_libraries(Config_TOML_Types PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        tomlplusplus::tomlplusplus)
