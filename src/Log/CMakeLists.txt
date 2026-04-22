project(Log VERSION 0.1.0 LANGUAGES CXX)

add_library(Log
        Log.h)

target_include_directories(Log PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})

target_link_libraries(Log PUBLIC
        CppUtils
        Qt${QT_VERSION_MAJOR}::Core)

# If enabled, print debug-level logs in all files that include this library
if(LOG_DEBUG)
        target_compile_definitions(Log PUBLIC LOG_DEBUG)
endif()
