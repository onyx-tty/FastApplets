cmake_minimum_required(VERSION 3.16)

project(power-applet VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# TODO Build type checking to disable sanitizer for release versions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

set(QT_VERSION_MAJOR 6)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
# TODO Qt5 fallback support
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

set(INCLUDED_HEADERS
        ../../MainWindow.h
        ../../Config/SharedLayout.h
        ../../Widgets/Button.h
)

set(INCLUDED_SOURCES
        ../../MainWindow.cpp
        ../../Config/SharedLayout.cpp
        ../../Widgets/Button.cpp
)

set(PROJECT_HEADERS
        PowerMainWindow.h
        ../../Config/PowerLayout.h
        Widgets/PowerButton.h
        Widgets/ButtonList.h
)

set(PROJECT_SOURCES
        main.cpp
        PowerMainWindow.cpp
        ../../Config/PowerLayout.cpp
        Widgets/ButtonList.cpp
)

set(PROJECT_RESOURCES

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
