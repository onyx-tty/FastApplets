add_library(PowerApplet_UI_Widgets
        PowerButton.cpp
        PowerButtonRecord.h)
add_library(PowerApplet::UI::Widgets ALIAS PowerApplet_UI_Widgets)

target_include_directories(PowerApplet_UI_Widgets PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(PowerApplet_UI_Widgets PRIVATE
        UI::Enums
        UI::Widgets
        Config::ConfigFile::Properties
        Config::ConfigFile::Global
        Config::ConfigFile::PowerApplet
        Config::KeysFile::Global
        Config::KeysFile::PowerApplet
        DBus
        Shell)

target_link_libraries(PowerApplet_UI_Widgets PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
