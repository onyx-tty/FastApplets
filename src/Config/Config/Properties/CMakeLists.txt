add_library(Config_Config_Properties
        WindowProperties.cpp
        PrimaryButtonProperties.cpp
        LayoutProperties.cpp
        EnvironmentProperties.cpp)
add_library(Config::Config::Properties ALIAS Config_Config_Properties)

target_include_directories(Config_Config_Properties PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Config_Config_Properties PRIVATE
        UI::Widgets)

target_link_libraries(Config_Config_Properties PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
