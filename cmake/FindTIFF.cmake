# TIFF_FOUND - TIFF was found on the system
# TIFF_INCLUDE_DIRS - GLEW include directories
# TIFF_LIBRARIES - GLEW import library

if (TIFF_INCLUDE_DIRS AND TIFF_LIBRARIES)
    set(TIFF_FOUND TRUE)
else ()
    # Find the TIFF library
    find_path(TIFF_INCLUDE_DIRS tiff.h
              PATHS /usr/include
                    /usr/local/include
                    /opt/include
                    "C:\\Program Files (x86)\\GnuWin32\\include"
                    "C:\\Program Files\\GnuWin32\\include"
    )
    find_library(TIFF_LIBRARIES libtiff
                 PATHS /usr/lib
                       /usr/lib64
                       "C:\\Program Files (x86)\\GnuWin32\\lib"
                       "C:\\Program Files\\GnuWin32\\lib"
    )
    if(WIN32)
        find_file(TIFF_DLL_PATH libtiff3.dll
                  PATHS "C:\\Program Files (x86)\\GnuWin32\\bin"
                        "C:\\Program Files\\GnuWin32\\bin"
                  NO_DEFAULT_PATH
        )
        find_file(TIFF_ZLIB_DLL_PATH zlib1.dll
                  PATHS "C:\\Program Files (x86)\\GnuWin32\\bin"
                        "C:\\Program Files\\GnuWin32\\bin"
                  NO_DEFAULT_PATH
        )
        find_file(TIFF_JPEG_DLL_PATH jpeg62.dll
                  PATHS "C:\\Program Files (x86)\\GnuWin32\\bin"
                        "C:\\Program Files\\GnuWin32\\bin"
                  NO_DEFAULT_PATH
        )
    endif(WIN32)
    
    # Determine whether TIFF was found
    set(TIFF_FOUND FALSE)
    if (TIFF_INCLUDE_DIRS AND TIFF_LIBRARIES)
        set(TIFF_FOUND TRUE)
    endif()
endif ()

macro(copy_dll_to_bin_dir DLL)
    if(EXISTS "${DLL}")
        file(COPY "${DLL}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
        file(COPY "${DLL}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
     endif()
endmacro()

if(TIFF_FOUND)
    if(NOT TIFF_FIND_QUIETLY)
        message(STATUS "Found TIFF: ${TIFF_LIBRARIES}")
    endif(NOT TIFF_FIND_QUIETLY)
    if(WIN32)
        copy_dll_to_bin_dir("${TIFF_DLL_PATH}")
        copy_dll_to_bin_dir("${TIFF_ZLIB_DLL_PATH}")
        copy_dll_to_bin_dir("${TIFF_JPEG_DLL_PATH}")
    endif()
elseif(TIFF_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find TIFF")
endif()
