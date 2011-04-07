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

    virtual int groupCount() const = 0;
    virtual uint groupMode(int index) const = 0;
    virtual uint groupSize(int index) const = 0;
    virtual void addGroup(VertexGroup *vg) = 0;
    virtual bool copyGroupTo(int index, VertexGroup *vg) const = 0;

    enum OutputMode
    {
        RenderToScreen,
        RenderToMesh
    };

    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0) = 0;
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    virtual void drawNormals(RenderState *s);
    virtual void saveStl(QString path) const;
    virtual void saveObj(QString path) const;

    static VertexGroup * loadStl(const char *path, bool computeNormals = false);
    static VertexGroup * loadObj(const char *path);
    static void saveStl(QString path, VertexGroup **vg, int groups);
    static void saveObj(QString path, VertexGroup **vg, int groups);

private:
    static void saveObjIndicesTri(FILE *f, VertexGroup *vg, uint &offset);
    static void saveObjIndicesQuad(FILE *f, VertexGroup *vg, uint &offset);
    static void saveObjIndicesTriStrip(FILE *f, VertexGroup *vg, uint &offset);
    static void saveObjFace(FILE *f, uint ind1, uint ind2, uint ind3);
    static void saveObjIndice(FILE *f, uint indice);
};

#endif
