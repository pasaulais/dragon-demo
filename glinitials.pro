INCLUDEPATH += include
HEADERS = include/SceneViewport.h \
    include/RenderState.h include/RenderStateGL1.h include/RenderStateGL2.h \
    include/Mesh.h include/Material.h include/Vertex.h \
    include/Dragon.h include/Scene.h

SOURCES = src/main.cpp src/SceneViewport.cpp \
    src/RenderState.cpp src/RenderStateGL1.cpp src/RenderStateGL2.cpp \
    src/Mesh.cpp src/Material.cpp src/Vertex.cpp \
    src/Scene.cpp src/Dragon.cpp
LIBS += -lm -ltiff

QMAKE_CFLAGS_DEBUG += -std=c99

TARGET = glinitials
CONFIG += qt warn_on debug thread
QT += opengl
