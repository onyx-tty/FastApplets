add_library(PowerApplet_UI
        ${POWERAPPLET_RESOURCES}
        PowerMainWindow.cpp
        PowerCentralWidget.cpp
        Widgets/PowerButton.cpp
        Input/KeyAction.cpp)
add_library(PowerApplet::UI ALIAS PowerApplet_UI)
target_include_directories(PowerApplet_UI PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${GLOBAL_INCLUDES})
target_link_libraries(PowerApplet_UI PRIVATE
        Config
        Core::Log
        UI
        PowerApplet::DBus)

