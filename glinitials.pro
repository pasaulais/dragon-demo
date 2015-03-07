INCLUDEPATH += include
HEADERS = include/SceneViewport.h \
    include/RenderState.h include/RenderStateGL1.h include/RenderStateGL2.h \
    include/Mesh.h include/Material.h include/Vertex.h \
    include/Dragon.h include/Scene.h \
    include/MeshGL1.h \
    include/MeshGL2.h \
    include/Platform.h

SOURCES = src/main.cpp src/SceneViewport.cpp \
    src/RenderState.cpp src/RenderStateGL1.cpp src/RenderStateGL2.cpp \
    src/Mesh.cpp src/Material.cpp src/Vertex.cpp \
    src/Scene.cpp src/Dragon.cpp \
    src/MeshGL1.cpp \
    src/MeshGL2.cpp \
    android/jni/gl_code.cpp \
    src/Platform.cpp
LIBS += -lm -ltiff -lGLEW -lGLU

QMAKE_CFLAGS_DEBUG += -std=c99

TARGET = glinitials
CONFIG += qt warn_on debug thread console
QT += opengl

OTHER_FILES += \
    fragment.glsl \
    vertex.glsl

RESOURCES += \
    resources.qrc
