add_subdirectory(Widgets)

add_library(PowerApplet_UI
        ${POWERAPPLET_RESOURCES}
        PowerMainWindow.cpp
        PowerCentralWidget.cpp)
add_library(PowerApplet::UI ALIAS PowerApplet_UI)

target_include_directories(PowerApplet_UI PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${GLOBAL_INCLUSIONS})

target_link_libraries(PowerApplet_UI PRIVATE
        PowerApplet::UI::Widgets
        PowerApplet::DBus
        Config::ConfigFile::Global
        Config::ConfigFile::PowerApplet
        Config::ConfigFile::Properties
        Config::KeysFile::Global
        Config::KeysFile::PowerApplet
        Config::KeysFile::Types
        Shell
        UI
        UI::Widgets)

target_link_libraries(PowerApplet_UI PUBLIC
        CppUtils::Log
        Qt${QT_VERSION_MAJOR}::Widgets)
