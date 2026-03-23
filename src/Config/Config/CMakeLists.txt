add_subdirectory(Global)
add_subdirectory(PowerApplet)
add_subdirectory(Properties)

add_library(Config_Config
        ConfigMapper.cpp)
add_library(Config::Config ALIAS Config_Config)

target_include_directories(Config_Config PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Config PRIVATE
        Config::Config::Global
        Config::Config::PowerApplet
        Config::Config::Properties
        Config::TOML
        Core::Log
        UI::Enums
        UI::Widgets)

target_link_libraries(Config_Config PUBLIC
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
