add_library(Config_ConfigFile_Properties
        WindowProperties.cpp
        PrimaryButtonProperties.cpp
        LayoutProperties.cpp
        EnvironmentProperties.cpp)
add_library(Config::ConfigFile::Properties ALIAS Config_ConfigFile_Properties)

target_include_directories(Config_ConfigFile_Properties PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_ConfigFile_Properties PRIVATE
        UI::Widgets
        Log)

target_link_libraries(Config_ConfigFile_Properties PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
