add_library(Config_Config
        Config.cpp
        ConfigMapper.cpp)
add_library(Config::Config ALIAS Config_Config)

target_include_directories(Config_Config PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Config PRIVATE
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Config_Config PUBLIC
        Config::TOML
        Core::Log
        UI::Enums
        UI::Widgets)
