add_library(PowerApplet_UI_Widgets
        PowerButton.cpp)
add_library(PowerApplet::UI::Widgets ALIAS PowerApplet_UI_Widgets)

target_include_directories(PowerApplet_UI_Widgets PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(PowerApplet_UI_Widgets PRIVATE
        Core::Log
        DBus
        Environment
        UI::Enums)

target_link_libraries(PowerApplet_UI_Widgets PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
