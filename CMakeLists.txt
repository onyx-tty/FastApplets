cmake_minimum_required(VERSION 3.16)

project(FastApplets VERSION 1.0.0)

# Applets for building
option(BUILD_POWERAPPLET "Include PowerApplet in the building process" ON)
option(BUILD_ACTIONAPPLET "Include ActionApplet in the building process" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_VERSION_MAJOR 6)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install and use CPM to resolve conflicting and duplicated CppUtils which has
# been included in TomlQt as well
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=f3a6dcc6a04ce9e7f51a127307fa4f699fb2bade357a8eb4c5b45df76e1dc6a5)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

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

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Third-party
CPMAddPackage("gh:onyx-tty/CppUtils#cef444b")
CPMAddPackage("gh:onyx-tty/TomlQt#2cb232a")

add_subdirectory(resources)
add_subdirectory(Core)

if(BUILD_POWERAPPLET)
        add_subdirectory(PowerApplet)
endif()

if(BUILD_ACTIONAPPLET)
        add_subdirectory(ActionApplet)
endif()

if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
endif()

install(# Install compiled artifacts to correct destinations, register all targets
        # under the name FastApplets_export
        TARGETS Core_Config
                Core_Config_ConfigFile
                Core_Config_ConfigFile_Config Core_Config_ConfigFile_Mapper
                Core_Config_ConfigFile_Properties
                Core_Config_KeysFile
                Core_Config_KeysFile_Keys Core_Config_KeysFile_Mapper
                Core_Config_KeysFile_Types
                Core_Config_ConfigManager
                Core_Config_Factory
                Core_Config_FileLocator
                Core_Config_TomlParser
                Core_Config_Resolver Core_Config_Resolver_PathContext Core_Config_Resolver_Types
                Core_Config_Types
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

if(BUILD_POWERAPPLET)
        install(
                TARGETS PowerApplet
                        PowerApplet_Types
                EXPORT FastApplets_export
                ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(BUILD_ACTIONAPPLET)
        install(
                TARGETS ActionApplet
                        ActionApplet_Types
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
