cmake_minimum_required(VERSION 3.16)

project(PowerApplet VERSION 0.23.5 LANGUAGES CXX)

add_subdirectory(DBus)
add_subdirectory(UI)

# Executables
qt_add_executable(PowerApplet
        MANUAL_FINALIZATION
        main.cpp)

# Target linking and configuration
target_include_directories(PowerApplet PRIVATE
        ${SRC_DIR})

target_link_libraries(PowerApplet PRIVATE
        Config
        DBus
        UI
        PowerApplet::UI
        PowerApplet::DBus)

target_link_libraries(PowerApplet PUBLIC
        CppUtils::Log::Qt)

if (ENABLE_ASAN OR ENABLE_LSAN OR ENABLE_UBSAN)
        target_compile_options(PowerApplet PRIVATE -fno-omit-frame-pointer -g)
endif()

if(ENABLE_ASAN)
        target_compile_options(PowerApplet PRIVATE -fsanitize=address)
        target_link_options(PowerApplet PRIVATE -fsanitize=address)
endif()

if(ENABLE_LSAN)
        target_compile_options(PowerApplet PRIVATE -fsanitize=leak)
        target_link_options(PowerApplet PRIVATE -fsanitize=leak)
endif()

if(ENABLE_UBSAN)
        target_compile_options(PowerApplet PRIVATE -fsanitize=undefined)
        target_link_options(PowerApplet PRIVATE -fsanitize=undefined)
endif()

target_compile_definitions(PowerApplet PUBLIC CPPUTILS_ENABLE_QT)

install(TARGETS PowerApplet
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Finalization
qt_finalize_executable(PowerApplet)
