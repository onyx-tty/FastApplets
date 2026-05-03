add_library(Config_ConfigFile_Global
        GlobalConfig.cpp)
add_library(Config::ConfigFile::Global ALIAS Config_ConfigFile_Global)

target_include_directories(Config_ConfigFile_Global PRIVATE
        ${SRC_DIR})

target_link_libraries(Config_ConfigFile_Global PRIVATE
        Config::ConfigFile::Properties
        Config::ConfigFile::Mapper
        Shell)

target_link_libraries(Config_ConfigFile_Global PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
