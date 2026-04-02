add_library(AppletTypes
        AppletTypes.h)

target_include_directories(AppletTypes PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${GLOBAL_INCLUSIONS})
