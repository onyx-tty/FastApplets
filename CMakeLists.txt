cmake_minimum_required(VERSION 3.16)

project(FastApplets VERSION 3.0.0)

option(BUILD_POWER_APPLET "Include PowerApplet in the building process" ON)
option(BUILD_ACTION_APPLET "Include ActionApplet in the building process" ON)
option(BUILD_PLAYER_APPLET "Include PlayerApplet in the building process" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_VERSION_MAJOR 6)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(cmake/AddTarget.cmake)
include(cmake/Subdirectory.cmake)

# Install and use CPM to resolve conflicting and duplicated CppUtils which has
# been included in TomlQt as well
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=f3a6dcc6a04ce9e7f51a127307fa4f699fb2bade357a8eb4c5b45df76e1dc6a5)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(tomlplusplus REQUIRED)

if(ENABLE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY
                clang-tidy;
                -header-filter=.;)
endif()

# Export compile_commands.json to ensure language servers work properly
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build static by default
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(BUILD_TESTS "Build unit tests" ON)
set(CPPUTILS_ENABLE_QT ON)

# Third-party
CPMAddPackage("gh:onyx-tty/CppUtils#cef444b")
CPMAddPackage("gh:onyx-tty/TomlQt#f4fdb5b")

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectories(resources Core)

if(BUILD_POWER_APPLET)
        add_subdirectory(PowerApplet)
endif()

if(BUILD_ACTION_APPLET)
        add_subdirectory(ActionApplet)
endif()

if(BUILD_PLAYER_APPLET)
        add_subdirectory(PlayerApplet)
endif()

if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
endif()

install(# Install compiled artifacts to correct destinations, register all targets
        # under the name FastApplets_export
        TARGETS Core_Config
                Core_Config_Schema
                Core_Config_Map Core_Config_Map_Helpers
                Core_Config_AppletConfig
                Core_Config_Defaults Core_Config_Defaults_Helpers
                Core_Config_Locate
                Core_Config_Toml
                Core_Config_Resolve
                Core_Config_Log
                Core_Config_Select
                Core_Config_Types
                Core_Config_Types_Candidates Core_Config_Types_PathContext
                Core_Shell
                Core_UI
                Core_UI_Widgets Core_UI_Types
                Core_Applets_Types
                Resources_Icons
        EXPORT FastApplets_export
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(BUILD_POWER_APPLET)
        install(
                TARGETS PowerApplet
                        PowerApplet_Types
                EXPORT FastApplets_export
                ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(BUILD_ACTION_APPLET)
        install(
                TARGETS ActionApplet
                        ActionApplet_Types
                EXPORT FastApplets_export
                ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(BUILD_PLAYER_APPLET)
        install(
                TARGETS PlayerApplet
                        PlayerApplet_Types
                EXPORT FastApplets_export
                ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# Write resulting FastApplets_export to FastApplets_export.cmake
install(EXPORT FastApplets_export
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/FastApplets)

# Process the Config.cmake.in template, write FastAppletsConfig.cmake
# to the build directory
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/FastAppletsConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/FastApplets")

# Generate FastAppletsConfigVersion.cmake to ensure version
# requirements are satisfied by the consumer
write_basic_package_version_file(
        FastAppletsConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion)

# Copy both generated cmake files into the install tree
install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/FastAppletsConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/FastAppletsConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/FastApplets)
