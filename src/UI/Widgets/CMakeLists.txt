add_library(UI_Widgets
        PrimaryButton.cpp)
add_library(UI::Widgets ALIAS UI_Widgets)

target_include_directories(UI_Widgets PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(UI_Widgets PRIVATE
        Config::ConfigFile::Properties
        UI::Types) # PowerButtonParams

target_link_libraries(UI_Widgets PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
