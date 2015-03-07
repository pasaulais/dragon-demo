# GLEW_FOUND - GLEW was found on the system
# GLEW_INCLUDE_DIRS - GLEW include directories
# GLEW_LIBRARIES - GLEW import library

set(GLEW_INTREE_DIR "${DEMO_DEPENDS_PATH_DEPENDS_PATH}/glew-${GLEW_FIND_VERSION}")
set(GLEW_INTREE_DLL "${GLEW_INTREE_DIR}/bin/glew32.dll")
set(GLEW_ARCHIVE "${GLEW_INTREE_DIR}-win32.zip")

if (GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
    set(GLEW_FOUND TRUE)
else ()
    # Try to unpack the GLEW archive if the directory doesn't exist.
    if(NOT EXISTS ${GLEW_INTREE_DIR} AND EXISTS ${GLEW_ARCHIVE})
        file(MAKE_DIRECTORY ${GLEW_INTREE_DIR})
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${GLEW_ARCHIVE}
                        WORKING_DIRECTORY ${EQLI_DEPENDS_PATH})
    endif()

    # Find the GLEW library
    find_path(GLEW_INCLUDE_DIRS GL/glew.h
              PATHS /usr/include /usr/local/include /opt/include "${GLEW_INTREE_DIR}/include"
    )
    find_library(GLEW_LIBRARIES glew32 GLEW
                 PATHS /usr/lib /usr/lib64 "${GLEW_INTREE_DIR}/lib"
    )
    
    # Determine whether GLEW was found
    set(GLEW_FOUND FALSE)
    if (GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
        set(GLEW_FOUND TRUE)
    endif()
endif ()

if(GLEW_FOUND)
    if(NOT GLEW_FIND_QUIETLY)
        message(STATUS "Found GLEW: ${GLEW_LIBRARIES}")
    endif(NOT GLEW_FIND_QUIETLY)
    if(WIN32 AND EXISTS "${GLEW_INTREE_DLL}")
        file(COPY "${GLEW_INTREE_DLL}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
        file(COPY "${GLEW_INTREE_DLL}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
    endif()
elseif(GLEW_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find GLEW")
endif()
