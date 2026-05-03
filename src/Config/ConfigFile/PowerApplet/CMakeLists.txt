add_library(Config_ConfigFile_PowerApplet
        PowerAppletConfig.cpp)
add_library(Config::ConfigFile::PowerApplet ALIAS Config_ConfigFile_PowerApplet)

target_include_directories(Config_ConfigFile_PowerApplet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${SRC_DIR})

target_link_libraries(Config_ConfigFile_PowerApplet PRIVATE
        Config::ConfigFile::Mapper
        Config::ConfigFile::Properties
        Shell)

target_link_libraries(Config_ConfigFile_PowerApplet PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
