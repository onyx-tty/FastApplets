add_library(Config_Config_Global
        GlobalConfig.cpp)
add_library(Config::Config::Global ALIAS Config_Config_Global)

target_include_directories(Config_Config_Global PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Config_Global PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Config_Config_Global PUBLIC
        Config::Config::Properties
        Environment)
