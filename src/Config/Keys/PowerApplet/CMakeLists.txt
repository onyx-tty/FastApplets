add_library(Config_Keys_PowerApplet
        PowerAppletKeys.cpp)
add_library(Config::Keys::PowerApplet ALIAS Config_Keys_PowerApplet)

target_include_directories(Config_Keys_PowerApplet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Keys_PowerApplet PRIVATE
        Config::Keys::Keybindings
        Config::TOML)

target_link_libraries(Config_Keys_PowerApplet PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
