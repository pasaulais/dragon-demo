cmake_minimum_required(VERSION 2.8)

project(DragonDemo)

set(DEMO_DEPENDS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/depends")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")

if(WIN32)
    file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
    file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
endif(WIN32)

find_package(Qt4 4.6.0 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
find_package(GLEW 1.9.0 REQUIRED)
find_package(TIFF 3.8.2 REQUIRED)
include(${QT_USE_FILE})

if( WIN32 )
    set(GL_LIBRARIES OpenGL32.lib GLU.lib GLEW.lib)
    set(SYSTEM_LIBRARIES tiff.lib)
else( WIN32 )
    set(GL_LIBRARIES GL GLU GLEW)
    set(SYSTEM_LIBRARIES dl pthread tiff)
endif( WIN32 )

if(WIN32)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/win32")
    add_definitions(/DM_PI=3.14159265358979323846)
endif(WIN32)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

subdirs(src)
