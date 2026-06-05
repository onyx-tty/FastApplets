add_library(Core_UI_Types
        ButtonType.cpp)
add_library(Core::UI::Types ALIAS Core_UI_Types)

target_include_directories(Core_UI_Types PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_UI_Types PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
