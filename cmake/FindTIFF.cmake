# TIFF_FOUND - TIFF was found on the system
# TIFF_INCLUDE_DIRS - GLEW include directories
# TIFF_LIBRARIES - GLEW import library

set(TIFF_INTREE_DIR "${DEMO_DEPENDS_PATH}/tiff-${TIFF_FIND_VERSION}-1")
set(TIFF_INTREE_DLL "${TIFF_INTREE_DIR}/bin/tiff.dll")
set(TIFF_ARCHIVE "${TIFF_INTREE_DIR}-win32.zip")

if (TIFF_INCLUDE_DIRS AND TIFF_LIBRARIES)
    set(TIFF_FOUND TRUE)
else ()
    # Try to unpack the TIFF archive if the directory doesn't exist.
    if(NOT EXISTS ${TIFF_INTREE_DIR} AND EXISTS ${TIFF_ARCHIVE})
        file(MAKE_DIRECTORY ${TIFF_INTREE_DIR})
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${TIFF_ARCHIVE}
                        WORKING_DIRECTORY ${EQLI_DEPENDS_PATH})
    endif()

    # Find the TIFF library
    find_path(TIFF_INCLUDE_DIRS tiff.h
              PATHS /usr/include /usr/local/include /opt/include "${TIFF_INTREE_DIR}/include"
    )
    find_library(TIFF_LIBRARIES libtiff
                 PATHS /usr/lib /usr/lib64 "${TIFF_INTREE_DIR}/lib"
    )
    
    # Determine whether TIFF was found
    set(TIFF_FOUND FALSE)
    if (TIFF_INCLUDE_DIRS AND TIFF_LIBRARIES)
        set(TIFF_FOUND TRUE)
    endif()
endif ()

if(TIFF_FOUND)
    if(NOT TIFF_FIND_QUIETLY)
        message(STATUS "Found TIFF: ${TIFF_LIBRARIES}")
    endif(NOT TIFF_FIND_QUIETLY)
    if(WIN32 AND EXISTS "${TIFF_INTREE_DLL}")
        file(COPY "${TIFF_INTREE_DLL}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
        file(COPY "${TIFF_INTREE_DLL}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
    endif()
elseif(TIFF_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find TIFF")
endif()
