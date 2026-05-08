add_library(PowerApplet_UI_Widgets
        PowerButton.cpp
        PowerButtonRecord.h)
add_library(PowerApplet::UI::Widgets ALIAS PowerApplet_UI_Widgets)

target_include_directories(PowerApplet_UI_Widgets PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_UI_Widgets PRIVATE
        UI::Types
        UI::Widgets
        Config::ConfigFile::Properties
        Config::ConfigFile::Global
        Config::ConfigFile::PowerApplet
        Config::KeysFile::Global
        Config::KeysFile::PowerApplet
        Shell)

target_link_libraries(PowerApplet_UI_Widgets PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
