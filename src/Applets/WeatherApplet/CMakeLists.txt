cmake_minimum_required(VERSION 3.16)

project(weather-applet VERSION 0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Enable Qt support for QObject MOC and resource files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# TODO Build type checking to disable sanitizer for release versions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

# Set up Qt environment
set(QT_VERSION_MAJOR 6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# TODO Qt5 fallback support
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

# Set up libcurl
find_package(CURL REQUIRED)
include_directories(weather-applet PRIVATE ${CURL_INCLUDE_DIR})

# Configure TraverseJSON to enable Qt support
set(TRAVERSEJSON_ENABLE_QT ON CACHE BOOL "Enable Qt in TraverseJSON")

# TODO Rework to utilize ${PROJECT_ROOT_DIR}, which will require better CMakeLists management, including modularization
# TODO Use FetchContent to skip git submodule and additional work for the user
# Add submodules
add_subdirectory(
        ../../../modules/TraverseJSON
        ../../../build/TraverseJSON-build
)

set(INCLUDED_HEADERS
        ../../MainWindow.h
        ../../Widgets/Button.h
        ../../Config/SharedLayout.h
        ../../Config/SharedKeybindings.h
        ../../Config/WeatherLayout.h
        ../../API/Curl.h
        ../../Utils/Time.h
        ../../Utils/TimeConstants.h
)

set(INCLUDED_SOURCES
        ../../MainWindow.cpp
        ../../Widgets/Button.cpp
        ../../Config/SharedLayout.cpp
        ../../Config/SharedKeybindings.cpp
        ../../Config/WeatherLayout.cpp
        ../../API/Curl.cpp
        ../../Utils/Time.cpp
)

set(PROJECT_HEADERS
        WeatherMainWindow.h
        CentralWidget.h
        API/OpenWeatherAPI.h
        API/WeatherData.h
        API/WeatherParser.h
        Utils/PresentTimeManager.h
)

set(PROJECT_SOURCES
        main.cpp
        WeatherMainWindow.cpp
        CentralWidget.cpp
        API/OpenWeatherAPI.cpp
        API/WeatherData.cpp
        API/WeatherParser.cpp
        Utils/PresentTimeManager.cpp
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

# Maintain support for both Qt6 and >=Qt5 for future expansion
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
        qt_add_executable(weather-applet
                MANUAL_FINALIZATION
                ${CODEBASE}
        )
else()
        add_executable(weather-applet
                ${CODEBASE}
        )
endif()

# Linked libraries
target_link_libraries(weather-applet PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        ${CURL_LIBRARIES}
        TraverseJSON
)

include(GNUInstallDirs)
install(TARGETS weather-applet
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(weather-applet)
endif()
