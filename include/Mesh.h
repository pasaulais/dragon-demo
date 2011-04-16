#ifndef INITIALS_MESH_H
#define INITIALS_MESH_H

#include <cstdio>
#include <string>
#include <iostream>
#include <inttypes.h>
#include "Vertex.h"

using namespace std;

class RenderState;

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    virtual int groupCount() const = 0;
    virtual uint32_t groupMode(int index) const = 0;
    virtual uint32_t groupSize(int index) const = 0;
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
    virtual void saveStl(string path) const;
    virtual void saveObj(string path) const;

    static VertexGroup * loadStl(string path, bool computeNormals = false);
    static VertexGroup * loadObj(string path);
    static VertexGroup * loadObj(istream &s);
    static void saveStl(string path, VertexGroup **vg, int groups);
    static void saveObj(string path, VertexGroup **vg, int groups);

private:
    static void saveObjIndicesTri(FILE *f, VertexGroup *vg, uint32_t &offset);
    static void saveObjIndicesQuad(FILE *f, VertexGroup *vg, uint32_t &offset);
    static void saveObjIndicesTriStrip(FILE *f, VertexGroup *vg, uint32_t &offset);
    static void saveObjFace(FILE *f, uint32_t ind1, uint32_t ind2, uint32_t ind3);
    static void saveObjIndice(FILE *f, uint32_t indice);
};

#endif
