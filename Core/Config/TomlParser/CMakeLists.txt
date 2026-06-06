add_library(Core_Config_TomlParser
        TomlParser.cpp)
add_library(Core::Config::TomlParser ALIAS Core_Config_TomlParser)

target_include_directories(Core_Config_TomlParser PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_TomlParser PRIVATE
        Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(Core_Config_TomlParser PUBLIC
        tomlplusplus::tomlplusplus)
