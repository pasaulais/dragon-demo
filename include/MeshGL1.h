#ifndef INITIALS_MESH_GL1_H
#define INITIALS_MESH_GL1_H

#include <vector>
#include <inttypes.h>
#include "Mesh.h"

typedef struct
{
    uint32_t mode;
    int count;
    int offset;   // location of the face vertices in the mesh's indices array
    bool draw;
} Face;

class MeshGL1 : public Mesh
{
public:
    MeshGL1();
    virtual ~MeshGL1();

    virtual int groupCount() const;
    virtual uint32_t groupMode(int index) const;
    virtual uint32_t groupSize(int index) const;
    virtual void addGroup(VertexGroup *vg);
    virtual bool copyGroupTo(int index, VertexGroup *vg) const;
    void addFace(uint32_t mode, int vertexCount, int offset, bool draw = true);

    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0);
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    virtual void drawNormals(RenderState *s);

private:
    void drawVertexList();
    void drawToMesh(Mesh *m, RenderState *s);
    VertexGroup * drawFaceToMeshCopy(RenderState *s, Face f);

    std::vector<vec3> m_vertices;
    std::vector<vec3> m_normals;
    std::vector<vec2> m_texCoords;
    std::vector<Face> m_faces;
};

#endif
