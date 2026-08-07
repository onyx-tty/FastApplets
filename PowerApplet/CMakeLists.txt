project(PowerApplet VERSION 0.36.8 LANGUAGES CXX)

add_subdirectory(Types)

add_applet_qt_executable(PowerApplet
        MANUAL_FINALIZATION

        SOURCES main.cpp

        PRIVATE_DEPS PowerApplet::Types
                     Core::UI
                     Core::Args
                     Core::Config::AppletConfig
                     Core::Config::Locate
                     Core::Config::Types
                     Core::Applets::Types

        PUBLIC_DEPS CppUtils::Log::Qt
                    Qt${QT_VERSION_MAJOR}::Widgets)

if(ENABLE_UBSAN)
        target_compile_options(PowerApplet PRIVATE -fno-omit-frame-pointer -g -fsanitize=undefined)
        target_link_options(PowerApplet PRIVATE -fsanitize=undefined)
endif()

target_compile_definitions(PowerApplet PUBLIC CPPUTILS_ENABLE_QT)

install(TARGETS PowerApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

qt_finalize_executable(PowerApplet)
