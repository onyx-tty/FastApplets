cmake_minimum_required(VERSION 3.16)

project(PowerApplet VERSION 0.22.11 LANGUAGES CXX)

add_subdirectory(DBus)
add_subdirectory(UI)

# Executables
qt_add_executable(PowerApplet
        MANUAL_FINALIZATION
        ${POWERAPPLET_SOURCE_DIR}/main.cpp)

# Target linking and configuration
target_include_directories(PowerApplet PRIVATE
        ${GLOBAL_INCLUSIONS})

target_link_libraries(PowerApplet PRIVATE
        PowerApplet::Deps)

if (ENABLE_ASAN OR ENABLE_LSAN OR ENABLE_UBSAN)
        target_compile_options(PowerApplet PRIVATE -fno-omit-frame-pointer -g)
endif()

if(ENABLE_ASAN)
        target_compile_options(PowerApplet PRIVATE -fsanitize=address)
        target_link_options(PowerApplet PRIVATE -fsanitize=address)
endif()

if(ENABLE_LSAN)
        target_compile_options(PowerApplet PRIVATE -fsanitize=leak)
        target_link_options(PowerApplet PRIVATE -fsanitize=leak)
endif()

if(ENABLE_UBSAN)
        target_compile_options(PowerApplet PRIVATE -fsanitize=undefined)
        target_link_options(PowerApplet PRIVATE -fsanitize=undefined)
endif()

install(TARGETS PowerApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Finalization
qt_finalize_executable(PowerApplet)

# Internal dependencies
add_library(PowerApplet_Deps)
add_library(PowerApplet::Deps ALIAS PowerApplet_Deps)

target_link_libraries(PowerApplet_Deps INTERFACE
        Config
        DBus
        Log
        UI
        PowerApplet::UI
        PowerApplet::DBus)
