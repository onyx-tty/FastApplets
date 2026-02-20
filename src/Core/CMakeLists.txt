project(Core VERSION 0.4.0 LANGUAGES CXX)

add_library(Core_Log
        Log.h)
add_library(Core::Log ALIAS Core_Log)
target_include_directories(Core_Log PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUDES})
target_link_libraries(Core_Log PUBLIC
        Qt${QT_VERSION_MAJOR}::Core)
# If enabled, print debug-level logs in all files that include this library
if(LOG_DEBUG)
        target_compile_definitions(Core_Log PUBLIC LOG_DEBUG)
endif()

add_library(Core)
target_link_libraries(Core INTERFACE
        Core_Log)
