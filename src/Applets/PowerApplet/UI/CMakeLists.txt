add_library(power_ui
        ${POWERAPPLET_RESOURCES}
        PowerMainWindow.cpp
        PowerCentralWidget.cpp
        Widgets/PowerButton.cpp
        Input/KeyAction.cpp)
add_library(PowerApplet::UI ALIAS power_ui)
target_include_directories(power_ui PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${GLOBAL_INCLUDES})
target_link_libraries(power_ui PRIVATE
        Config
        LogUtils
        UI
        PowerApplet::DBus) # Power actions on button press, so DBus module is needed

