INCLUDEPATH += include
HEADERS = include/SceneViewport.h include/SimulationWindow.h \
    include/Dragon.h include/DragonScene.h include/Mesh.h \
    include/Scene.h include/Letters.h include/Primitive.h include/Images.h
SOURCES = src/main.cpp src/SceneViewport.cpp src/SimulationWindow.cpp \
    src/Dragon.cpp src/DragonScene.cpp src/Mesh.cpp \
    src/Scene.cpp src/Letters.cpp src/Primitive.cpp src/Images.cpp
LIBS += -lm -ltiff

QMAKE_CFLAGS_DEBUG += -std=c99

TARGET = glinitials
CONFIG += qt warn_on debug thread
QT += opengl
