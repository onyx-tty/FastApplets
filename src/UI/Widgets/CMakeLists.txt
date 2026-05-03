add_library(UI_Widgets
        PrimaryButton.cpp
        PrimaryButtonParams.h
        PowerButtonParams.h)
add_library(UI::Widgets ALIAS UI_Widgets)

target_include_directories(UI_Widgets PRIVATE
        ${SRC_DIR})

target_link_libraries(UI_Widgets PRIVATE
        Config::ConfigFile::Properties)

target_link_libraries(UI_Widgets PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
