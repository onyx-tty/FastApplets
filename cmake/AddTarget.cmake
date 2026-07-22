include(CMakeParseArguments)

function(add_applet_library NAME)
        set(OPTIONS)
        set(SINGLE_VALUE_ARGS TYPE)
        set(MULTI_VALUE_ARGS SOURCES PRIVATE_DEPS PUBLIC_DEPS INTERFACE_DEPS)

        cmake_parse_arguments(ARG "${OPTIONS}"
                                  "${SINGLE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}"
                                  ${ARGN})

        if(NOT ROOT_DIR)
                message(FATAL_ERROR "ROOT_DIR has not been defined")
        endif()

        set(ALIAS_NAME)
        string(REPLACE "_" "::" ALIAS_NAME ${NAME})

        add_library(${NAME} ${ARG_TYPE} ${ARG_SOURCES})
        add_library(${ALIAS_NAME} ALIAS ${NAME})

        if(ARG_TYPE STREQUAL "INTERFACE")
                target_include_directories(${NAME} INTERFACE
                        $<BUILD_INTERFACE:${ROOT_DIR}>
                        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

                if(ARG_INTERFACE_DEPS)
                        target_link_libraries(${NAME} INTERFACE ${ARG_INTERFACE_DEPS})
                endif()

                if(ARG_PRIVATE_DEPS)
                        message(FATAL_ERROR "PRIVATE deps are not allowed in INTERFACE libraries")
                endif()

                if(ARG_PUBLIC_DEPS)
                        message(FATAL_ERROR "PUBLIC deps are not allowed in INTERFACE libraries")
                endif()

                return()
        endif()

        target_include_directories(${NAME} PRIVATE
                $<BUILD_INTERFACE:${ROOT_DIR}>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

        if(ARG_INTERFACE_DEPS)
                message(FATAL_ERROR "INTERFACE deps are not allowed in non-INTERFACE libraries")
        endif()

        if(ARG_PRIVATE_DEPS)
                target_link_libraries(${NAME} PRIVATE ${ARG_PRIVATE_DEPS})
        endif()

        if(ARG_PUBLIC_DEPS)
                target_link_libraries(${NAME} PUBLIC ${ARG_PUBLIC_DEPS})
        endif()
endfunction()

function(add_applet_executable NAME)
        set(OPTIONS)
        set(SINGLE_VALUE_ARGS)
        set(MULTI_VALUE_ARGS SOURCES PRIVATE_DEPS PUBLIC_DEPS)

        cmake_parse_arguments(ARG "${OPTIONS}"
                                  "${SINGLE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}"
                                  ${ARGN})

        if(NOT ROOT_DIR)
                message(FATAL_ERROR "ROOT_DIR has not been defined")
        endif()

        add_executable(${NAME} ${ARG_SOURCES})

        target_include_directories(${NAME} PRIVATE
                $<BUILD_INTERFACE:${ROOT_DIR}>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

        if(ARG_PRIVATE_DEPS)
                target_link_libraries(${NAME} PRIVATE ${ARG_PRIVATE_DEPS})
        endif()

        if(ARG_PUBLIC_DEPS)
                target_link_libraries(${NAME} PUBLIC ${ARG_PUBLIC_DEPS})
        endif()
endfunction()

function(add_applet_qt_executable NAME)
        set(OPTIONS MANUAL_FINALIZATION)
        set(SINGLE_VALUE_ARGS)
        set(MULTI_VALUE_ARGS SOURCES PRIVATE_DEPS PUBLIC_DEPS)

        cmake_parse_arguments(ARG "${OPTIONS}"
                                  "${SINGLE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}"
                                  ${ARGN})

        if(NOT ROOT_DIR)
                message(FATAL_ERROR "ROOT_DIR has not been defined")
        endif()

        set(MANUAL_FINALIZATION)
        if(ARG_MANUAL_FINALIZATION)
                set(MANUAL_FINALIZATION MANUAL_FINALIZATION)
        endif()

        qt_add_executable(${NAME} ${MANUAL_FINALIZATION} ${ARG_SOURCES})

        target_include_directories(${NAME} PRIVATE
                $<BUILD_INTERFACE:${ROOT_DIR}>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

        if(ARG_PRIVATE_DEPS)
                target_link_libraries(${NAME} PRIVATE ${ARG_PRIVATE_DEPS})
        endif()

        if(ARG_PUBLIC_DEPS)
                target_link_libraries(${NAME} PUBLIC ${ARG_PUBLIC_DEPS})
        endif()
endfunction()
