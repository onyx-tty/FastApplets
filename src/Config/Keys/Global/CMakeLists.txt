add_library(Config_Keys_Global
        GlobalKeys.cpp)
add_library(Config::Keys::Global ALIAS Config_Keys_Global)

target_include_directories(Config_Keys_Global PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Keys_Global PRIVATE
        Config::Keys::Keybindings)
