add_library(Core_Config_Toml
        Parse.cpp)
add_library(Core::Config::Toml ALIAS Core_Config_Toml)

target_include_directories(Core_Config_Toml PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Toml PRIVATE
        Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(Core_Config_Toml PUBLIC
        tomlplusplus::tomlplusplus)
