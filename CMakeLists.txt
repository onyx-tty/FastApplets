cmake_minimum_required(VERSION 3.16)

project(FastApplets VERSION 1.0.0)

# Applets for building
option(BUILD_POWERAPPLET "Include PowerApplet in the building process" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_VERSION_MAJOR 6)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install and use CPM to resolve conflicting and duplicated CppUtils
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=f3a6dcc6a04ce9e7f51a127307fa4f699fb2bade357a8eb4c5b45df76e1dc6a5)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# clang-tidy
if(ENABLE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY
                clang-tidy;
                -header-filter=.;)
endif()

# Export compile_commands.json to ensure language servers work properly
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build shared by default
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(BUILD_TESTS "Build unit tests" ON)
set(CPPUTILS_ENABLE_QT ON)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_DIR ${ROOT_DIR}/src)

# Third-party libraries
if(BUILD_POWERAPPLET)
        CPMAddPackage("gh:onyx-tty/CppUtils#926ee69")
        CPMAddPackage("gh:onyx-tty/TomlQt#9d7d517")
endif()

# Internals
add_subdirectory(
        resources)

add_subdirectory(
        src/Config)

add_subdirectory(
        src/Shell)

add_subdirectory(
        src/UI)

add_subdirectory(
        src/DBus)

add_subdirectory(
        src/Applets/Types)

if(BUILD_POWERAPPLET)
        add_subdirectory(
                src/Applets/PowerApplet)
endif()

#if(BUILD_TESTS)
#        enable_testing()
#        add_subdirectory(tests)
#endif()

install(# Install compiled artifacts to correct destinations, register all targets
        # under the name FastApplets_export
        TARGETS Config
                Config_ConfigFile
                Config_ConfigFile_Global Config_ConfigFile_PowerApplet
                Config_ConfigFile_Mapper Config_ConfigFile_Properties
                Config_KeysFile
                Config_KeysFile_Global Config_KeysFile_PowerApplet
                Config_KeysFile_Mapper Config_KeysFile_Types
                Config_FileLocator
                Config_TomlParser
                Config_Resolver
                Config_Types
                DBus
                Shell
                UI
                UI_Enums UI_Widgets
                AppletTypes
                Resources_Icons
        EXPORT FastApplets_export
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(BUILD_POWERAPPLET)
        install(
                TARGETS PowerApplet
                        PowerApplet_DBus
                        PowerApplet_UI
                        PowerApplet_UI_Widgets
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
