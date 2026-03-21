add_subdirectory(Keybindings)

add_library(Config_Keys
        Keys.cpp
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
        Config::TOML
        Core::Log)
