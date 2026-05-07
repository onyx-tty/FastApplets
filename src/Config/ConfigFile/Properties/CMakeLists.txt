add_library(Config_ConfigFile_Properties
        WindowProperties.cpp
        PrimaryButtonProperties.cpp
        LayoutProperties.cpp)
add_library(Config::ConfigFile::Properties ALIAS Config_ConfigFile_Properties)

target_include_directories(Config_ConfigFile_Properties PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_ConfigFile_Properties PRIVATE
        Resources::Icons)

target_link_libraries(Config_ConfigFile_Properties PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)
