cmake_minimum_required(VERSION 3.16)

project(power-applet VERSION 0.10.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# TODO Build type checking to disable sanitizer for release versions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

set(QT_VERSION_MAJOR 6)

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus)
# TODO Qt5 fallback support
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets DBus)

set(INCLUDED_HEADERS
        ../../MainWindow.h
        ../../Config/SharedLayout.h
        ../../Config/SharedKeybindings.h
        ../../Widgets/Button.h
)

set(INCLUDED_SOURCES
        ../../MainWindow.cpp
        ../../Config/SharedLayout.cpp
        ../../Config/SharedKeybindings.cpp
        ../../Widgets/Button.cpp
)

set(PROJECT_HEADERS
        PowerMainWindow.h
        CentralWidget.h
        ../../Config/PowerLayout.h
        ../../Config/PowerKeybindings.h
        Widgets/PowerButton.h
        DBus/PowerActionManager.h
)

set(PROJECT_SOURCES
        main.cpp
        PowerMainWindow.cpp
        CentralWidget.cpp
        ../../Config/PowerLayout.cpp
        ../../Config/PowerKeybindings.cpp
        Widgets/PowerButton.cpp
        DBus/PowerActionManager.cpp
)

set(PROJECT_RESOURCES
        Data/Icons.qrc
)

set(CODEBASE
        ${INCLUDED_HEADERS}
        ${INCLUDED_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
        qt_add_executable(power-applet
                MANUAL_FINALIZATION
                ${CODEBASE}
        )
else()
        add_executable(power-applet
                ${CODEBASE}
        )
endif()

# Linked libraries
target_link_libraries(power-applet PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::DBus
)

include(GNUInstallDirs)
install(TARGETS power-applet
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(power-applet)
endif()
