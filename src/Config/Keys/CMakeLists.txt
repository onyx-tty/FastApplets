add_subdirectory(Keybindings)
add_subdirectory(Global)
add_subdirectory(PowerApplet)

add_library(Config_Keys
        KeysMapper.cpp)
add_library(Config::Keys ALIAS Config_Keys)

target_include_directories(Config_Keys PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Keys PRIVATE
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Config_Keys PUBLIC
        Config::Keys::Keybindings
        Config::Keys::Global
        Config::Keys::PowerApplet
        Config::TOML
        Core::Log)
