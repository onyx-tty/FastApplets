add_library(Core_Config_Types INTERFACE)
add_library(Core::Config::Types ALIAS Core_Config_Types)

target_include_directories(Core_Config_Types INTERFACE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Types INTERFACE
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Core)
