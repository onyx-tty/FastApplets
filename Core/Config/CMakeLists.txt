# Enable support for Qt's resource files
set(CMAKE_AUTORCC ON)

# Requirements
find_package(tomlplusplus REQUIRED)

add_subdirectory(ConfigFile)
add_subdirectory(KeysFile)
add_subdirectory(Manager)
add_subdirectory(FileLocator)
add_subdirectory(TomlParser)
add_subdirectory(Resolver)
add_subdirectory(Types)

add_library(Core_Config)
add_library(Core::Config ALIAS Core_Config)

target_include_directories(Core_Config PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config PRIVATE
        Core::Config::ConfigFile
        Core::Config::KeysFile
        Core::Config::FileLocator
        Core::Config::TomlParser
        Core::Config::Resolver
        Core::Config::Types
        Core::UI::Widgets)

target_link_libraries(Core_Config PUBLIC
        CppUtils::String
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Widgets)
