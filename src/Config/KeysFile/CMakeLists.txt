add_subdirectory(Keybindings)
add_subdirectory(Global)
add_subdirectory(PowerApplet)

add_library(Config_KeysFile
        KeysMapper.cpp)
add_library(Config::KeysFile ALIAS Config_KeysFile)

target_include_directories(Config_KeysFile PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_KeysFile PRIVATE
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Config_KeysFile PUBLIC
        Config::KeysFile::Keybindings
        Config::KeysFile::Global
        Config::KeysFile::PowerApplet
        Config::TOML
        Core::Log)
