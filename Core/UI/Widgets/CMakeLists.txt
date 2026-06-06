add_library(Core_UI_Widgets
        PrimaryButton.cpp)
add_library(Core::UI::Widgets ALIAS Core_UI_Widgets)

target_include_directories(Core_UI_Widgets PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_UI_Widgets PRIVATE
        Core::Config::ConfigFile::Properties)

target_link_libraries(Core_UI_Widgets PUBLIC
        Core::UI::Types # PowerButtonParams
        Qt${QT_VERSION_MAJOR}::Widgets)
