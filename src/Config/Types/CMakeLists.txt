add_library(Config_Types
        ConfigTomlFiles.h
        NodePair.h
        NodeView.h)
add_library(Config::Types ALIAS Config_Types)

target_include_directories(Config_Types PRIVATE
        ${SRC_DIR})

target_link_libraries(Config_Types PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        tomlplusplus::tomlplusplus)
