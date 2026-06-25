add_subdirectory(Widgets)
add_subdirectory(Types)

# Internal structure
add_library(Core_UI
        CentralWidget.cpp
        MainWindow.cpp)
add_library(Core::UI ALIAS Core_UI)

target_include_directories(Core_UI PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_UI PUBLIC
        Core::UI::Widgets
        Core::UI::Types
        Core::Config::ConfigFile::Properties
        Core::Config::KeysFile::Types
        Core::Applets::Types
        Qt${QT_VERSION_MAJOR}::Widgets)
