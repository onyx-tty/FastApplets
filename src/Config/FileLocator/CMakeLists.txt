add_library(Config_FileLocator
        FileLocator.cpp)
add_library(Config::FileLocator ALIAS Config_FileLocator)

target_include_directories(Config_FileLocator PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Config_FileLocator PRIVATE
        Config::Types)

target_link_libraries(Config_FileLocator PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Core)
