project(ActionApplet VERSION 0.11.8 LANGUAGES CXX)

add_subdirectory(Types)

add_applet_qt_executable(ActionApplet
        MANUAL_FINALIZATION

        SOURCES main.cpp

        PRIVATE_DEPS ActionApplet::Types
                     Core::UI
                     Core::Args
                     Core::Config::AppletConfig
                     Core::Config::Locate
                     Core::Config::Types
                     Core::Applets::Types

        PUBLIC_DEPS CppUtils::Log::Qt
                    Qt${QT_VERSION_MAJOR}::Widgets)

if(ENABLE_UBSAN)
        target_compile_options(ActionApplet PRIVATE -fno-omit-frame-pointer -g -fsanitize=undefined)
        target_link_options(ActionApplet PRIVATE -fsanitize=undefined)
endif()

target_compile_definitions(ActionApplet PUBLIC CPPUTILS_ENABLE_QT)

install(TARGETS ActionApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

qt_finalize_executable(ActionApplet)
