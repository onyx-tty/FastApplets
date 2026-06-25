project(PowerApplet VERSION 0.29.0 LANGUAGES CXX)

add_subdirectory(UI)
add_subdirectory(Types)

# Executables
qt_add_executable(PowerApplet
        MANUAL_FINALIZATION
        main.cpp)

# Target linking and configuration
target_include_directories(PowerApplet PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(PowerApplet PRIVATE
        PowerApplet::UI
        PowerApplet::Types
        Core::Config::ConfigManager
        Core::Config::Types
        Core::Applets::Types)

target_link_libraries(PowerApplet PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)

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
