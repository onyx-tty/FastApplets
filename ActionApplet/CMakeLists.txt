project(ActionApplet VERSION 0.7.0 LANGUAGES CXX)

add_subdirectory(UI)
add_subdirectory(Types)

# Executables
qt_add_executable(ActionApplet
        MANUAL_FINALIZATION
        main.cpp)

# Target linking and configuration
target_include_directories(ActionApplet PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ActionApplet PRIVATE
        ActionApplet::UI
        ActionApplet::Types
        Core::Config::ConfigManager
        Core::Config::Types
        Core::Applets::Types)

target_link_libraries(ActionApplet PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)

if(ENABLE_UBSAN)
        target_compile_options(ActionApplet PRIVATE -fno-omit-frame-pointer -g -fsanitize=undefined)
        target_link_options(ActionApplet PRIVATE -fsanitize=undefined)
endif()

target_compile_definitions(ActionApplet PUBLIC CPPUTILS_ENABLE_QT)

install(TARGETS ActionApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Finalization
qt_finalize_executable(ActionApplet)
