add_library(Core_Applets_Types INTERFACE)
add_library(Core::Applets::Types ALIAS Core_Applets_Types)

target_include_directories(Core_Applets_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Applets_Types INTERFACE
        Qt${QT_VERSION_MAJOR}::Core)
