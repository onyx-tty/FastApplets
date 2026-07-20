project(VolumeApplet VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(Types)

# Executables
qt_add_executable(VolumeApplet
        MANUAL_FINALIZATION
        main.cpp)

# Target linking and configuration
target_include_directories(VolumeApplet PRIVATE
        $<BUILD_INTERFACE:${ROOT_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(VolumeApplet PRIVATE
        VolumeApplet::Types
        Core::UI
        Core::Args
        Core::Config::AppletConfig
        Core::Config::Locate
        Core::Config::Types
        Core::Applets::Types)

target_link_libraries(VolumeApplet PUBLIC
        CppUtils::Log::Qt
        Qt${QT_VERSION_MAJOR}::Widgets)

if(ENABLE_UBSAN)
        target_compile_options(VolumeApplet PRIVATE -fno-omit-frame-pointer -g -fsanitize=undefined)
        target_link_options(VolumeApplet PRIVATE -fsanitize=undefined)
endif()

target_compile_definitions(VolumeApplet PUBLIC CPPUTILS_ENABLE_QT)

install(TARGETS VolumeApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Finalization
qt_finalize_executable(VolumeApplet)
