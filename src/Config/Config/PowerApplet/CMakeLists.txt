add_library(Config_Config_PowerApplet
        PowerAppletConfig.cpp)
add_library(Config::Config::PowerApplet ALIAS Config_Config_PowerApplet)

target_include_directories(Config_Config_PowerApplet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Config_PowerApplet PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Config_Config_PowerApplet PUBLIC
        Config::Config::Properties
        Environment)
