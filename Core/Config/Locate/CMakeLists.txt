add_library(Core_Config_Locate
        Locate.cpp)
add_library(Core::Config::Locate ALIAS Core_Config_Locate)

target_include_directories(Core_Config_Locate PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_Locate PRIVATE
        Core::Config::Types)

target_link_libraries(Core_Config_Locate PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
