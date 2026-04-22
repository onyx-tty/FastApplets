add_library(Config_ConfigFile_PowerApplet
        PowerAppletConfig.cpp)
add_library(Config::ConfigFile::PowerApplet ALIAS Config_ConfigFile_PowerApplet)

target_include_directories(Config_ConfigFile_PowerApplet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

# TODO Swap clauses
target_link_libraries(Config_ConfigFile_PowerApplet PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Config_ConfigFile_PowerApplet PUBLIC
        Config::ConfigFile::Mapper
        Config::ConfigFile::Properties
        Shell
        Log)
