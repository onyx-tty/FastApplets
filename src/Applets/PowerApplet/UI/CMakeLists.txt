add_subdirectory(Widgets)

add_library(PowerApplet_UI
        ${POWERAPPLET_RESOURCES}
        PowerMainWindow.cpp
        PowerCentralWidget.cpp
        Widgets/PowerButton.cpp)
add_library(PowerApplet::UI ALIAS PowerApplet_UI)

target_include_directories(PowerApplet_UI PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${GLOBAL_INCLUSIONS})

target_link_libraries(PowerApplet_UI PRIVATE
        Config
        Log
        Environment
        UI
        PowerApplet::DBus
        PowerApplet::UI::Widgets)

target_link_libraries(PowerApplet_UI PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
