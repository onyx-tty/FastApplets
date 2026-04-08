add_library(Config_KeysFile_PowerApplet
        PowerAppletKeys.cpp)
add_library(Config::KeysFile::PowerApplet ALIAS Config_KeysFile_PowerApplet)

target_include_directories(Config_KeysFile_PowerApplet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_KeysFile_PowerApplet PRIVATE
        Config::KeysFile::Keybindings
        Config::TOML)

target_link_libraries(Config_KeysFile_PowerApplet PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
