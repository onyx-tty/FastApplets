add_library(UI_Widgets
        PrimaryButton.cpp
        PrimaryButtonParams.h
        PowerButtonParams.h)
add_library(UI::Widgets ALIAS UI_Widgets)

target_include_directories(UI_Widgets PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(UI_Widgets PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
