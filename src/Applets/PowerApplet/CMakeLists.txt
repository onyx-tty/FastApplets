cmake_minimum_required(VERSION 3.16)

project(PowerApplet VERSION 0.23.9 LANGUAGES CXX)

add_subdirectory(UI)

# Executables
qt_add_executable(PowerApplet
        MANUAL_FINALIZATION
        main.cpp)

# Target linking and configuration
target_include_directories(PowerApplet PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet PRIVATE
        Config
        UI
        PowerApplet::UI)

target_link_libraries(PowerApplet PUBLIC
        CppUtils::Log::Qt)

if(ENABLE_UBSAN)
        target_compile_options(PowerApplet PRIVATE -fno-omit-frame-pointer -g -fsanitize=undefined)
        target_link_options(PowerApplet PRIVATE -fsanitize=undefined)
endif()

target_compile_definitions(PowerApplet PUBLIC CPPUTILS_ENABLE_QT)

install(TARGETS PowerApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Finalization
qt_finalize_executable(PowerApplet)
