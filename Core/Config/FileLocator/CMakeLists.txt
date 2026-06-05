add_library(Core_Config_FileLocator
        FileLocator.cpp)
add_library(Core::Config::FileLocator ALIAS Core_Config_FileLocator)

target_include_directories(Core_Config_FileLocator PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Config_FileLocator PRIVATE
        Core::Config::Types)

target_link_libraries(Core_Config_FileLocator PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
