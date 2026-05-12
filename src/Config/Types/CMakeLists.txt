add_library(Config_Types
        ConfigFiles.h
        ConfigType.h
        NodePair.h
        NodeView.h)
add_library(Config::Types ALIAS Config_Types)

target_include_directories(Config_Types PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_Types PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        tomlplusplus::tomlplusplus)
