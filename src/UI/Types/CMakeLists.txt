add_library(UI_Types
        ButtonType.cpp)
add_library(UI::Types ALIAS UI_Types)

target_include_directories(UI_Types PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(UI_Types PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
