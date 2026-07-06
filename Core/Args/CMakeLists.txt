add_library(Core_Args
        ArgumentParser.cpp)
add_library(Core::Args ALIAS Core_Args)

target_include_directories(Core_Args PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(Core_Args PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Core_Args PUBLIC
        Core::Applets::Types)
