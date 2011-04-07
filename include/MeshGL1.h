#ifndef INITIALS_MESH_GL1_H
#define INITIALS_MESH_GL1_H

#include <QObject>
#include <QVector>
#include <cstdio>
#include "Mesh.h"

typedef struct
{
    uint mode;
    int count;
    int offset;   // location of the face vertices in the mesh's indices array
    bool draw;
} FaceGL1;

class MeshGL1 : public Mesh
{
public:
    MeshGL1(QObject *parent = 0);
    virtual ~MeshGL1();

    QVector<vec3> & vertices();
    const QVector<vec3> & vertices() const;
    void setVertices(float *vertices, int n);

    QVector<vec3> & normals();
    const QVector<vec3> & normals() const;
    void setNormals(float *normals, int n);

    QVector<vec2> & texCoords();
    const QVector<vec2> & texCoords() const;
    void setTexCoords(float *texCoords, int n);

    const QVector<FaceGL1> & faces() const;

    virtual int groupCount() const;
    virtual uint groupMode(int index) const;
    virtual uint groupSize(int index) const;
    virtual void addGroup(VertexGroup *vg);
    virtual bool copyGroupTo(int index, VertexGroup *vg) const;
    void addFace(uint mode, int vertexCount, int offset, bool draw = true);

    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0);
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    virtual void drawNormals(RenderState *s);

private:
    void drawVertexList();
    void drawToMesh(Mesh *m, RenderState *s);
    VertexGroup * drawFaceToMeshCopy(RenderState *s, FaceGL1 f);

    QVector<vec3> m_vertices;
    QVector<vec3> m_normals;
    QVector<vec2> m_texCoords;
    QVector<FaceGL1> m_faces;
};

#endif
