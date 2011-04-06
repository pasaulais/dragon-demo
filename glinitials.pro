INCLUDEPATH += include
HEADERS = include/SceneViewport.h \
    include/Dragon.h include/Mesh.h \
    include/Scene.h include/Material.h \
    include/RenderState.h
SOURCES = src/main.cpp src/SceneViewport.cpp \
    src/Dragon.cpp src/Mesh.cpp \
    src/Scene.cpp src/Material.cpp \
    src/RenderState.cpp
LIBS += -lm -ltiff

QMAKE_CFLAGS_DEBUG += -std=c99

TARGET = glinitials
CONFIG += qt warn_on debug thread
QT += opengl
