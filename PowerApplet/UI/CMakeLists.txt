add_subdirectory(Widgets)

add_library(PowerApplet_UI
        PowerMainWindow.cpp
        PowerCentralWidget.cpp)
add_library(PowerApplet::UI ALIAS PowerApplet_UI)

target_include_directories(PowerApplet_UI PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet_UI PRIVATE
        PowerApplet::UI::Widgets
        Core::Config::ConfigFile::PowerApplet
        Core::Config::KeysFile::PowerApplet)

target_link_libraries(PowerApplet_UI PUBLIC
        PowerApplet::Types
        Core::UI
        Qt${QT_VERSION_MAJOR}::Widgets)
