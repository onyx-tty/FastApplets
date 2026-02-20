project(core VERSION 0.4.0 LANGUAGES CXX)

add_library(log_utils
        Log.h)
add_library(LogUtils ALIAS log_utils)
target_include_directories(log_utils PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})
target_link_libraries(log_utils PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
# If enabled, print debug-level logs in all files that include this library
if(LOG_DEBUG)
        target_compile_definitions(log_utils PUBLIC LOG_DEBUG)
endif()

add_library(core)
add_library(Core ALIAS core)
target_link_libraries(core INTERFACE
        log_utils)
