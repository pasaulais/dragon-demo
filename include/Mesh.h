#ifndef INITIALS_MESH_H
#define INITIALS_MESH_H

#include <QObject>
#include "Vertex.h"
#include <cstdio>

class RenderState;

class Mesh : public QObject
{
public:
    Mesh(QObject *parent = 0);
    virtual ~Mesh();

    virtual void addVertices(VertexGroup *vg) = 0;

    enum OutputMode
    {
        RenderToScreen,
        RenderToMesh
    };

    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0) = 0;
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    virtual void drawNormals(RenderState *s);

    static VertexGroup * loadStl(const char *path, bool computeNormals = false);
    static VertexGroup * loadObj(const char *path);
    virtual void saveStl(QString path) = 0;
    virtual void saveObj(QString path) = 0;
};

#endif
