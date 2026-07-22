# TODO: Add docs

function(add_subdirectories)
        if(NOT ARGN)
                message(WARNING "Empty add_subdirectories() does nothing")
        endif()

        foreach(NAME ${ARGN})
                add_subdirectory(${NAME})
        endforeach()
endfunction()
