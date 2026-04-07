add_library(Config_TOML_Types
        ConfigTomlFiles.h
        NodePair.h
        NodeView.h)
add_library(Config::TOML::Types ALIAS Config_TOML_Types)

target_include_directories(Config_TOML_Types PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_TOML_Types PRIVATE
        tomlplusplus::tomlplusplus)
