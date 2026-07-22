project(PlayerApplet VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(Types)

add_applet_qt_executable(PlayerApplet
        MANUAL_FINALIZATION

        SOURCES main.cpp

        PRIVATE_DEPS PlayerApplet::Types
                     Core::UI
                     Core::Args
                     Core::Config::AppletConfig
                     Core::Config::Locate
                     Core::Config::Types
                     Core::Applets::Types

        PUBLIC_DEPS CppUtils::Log::Qt
                    Qt${QT_VERSION_MAJOR}::Widgets)

if(ENABLE_UBSAN)
        target_compile_options(PlayerApplet PRIVATE -fno-omit-frame-pointer -g -fsanitize=undefined)
        target_link_options(PlayerApplet PRIVATE -fsanitize=undefined)
endif()

target_compile_definitions(PlayerApplet PUBLIC CPPUTILS_ENABLE_QT)

install(TARGETS PlayerApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Finalization
qt_finalize_executable(PlayerApplet)
