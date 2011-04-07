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

    QVector<uint> & indices();
    const QVector<uint> & indices() const;
    void setIndices(uint *indices, int n);

    const QVector<FaceGL1> & faces() const;

    virtual void addVertices(VertexGroup *vg);
    void addFace(uint mode, int vertexCount, int offset, bool draw = true);

    // Generate texture coordinates for 4 vertices-faced meshes
    void generateTextureCoords();

    virtual void computeNormals();

    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0);
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    virtual void drawNormals(RenderState *s);

    static MeshGL1 * loadStl(const char *path, QObject *parent = 0, bool compute_normals = true);
    static MeshGL1 * loadObj(const char *path, QObject *parent = 0);
    virtual void saveStl(QString path);
    virtual void saveObj(QString path);

private:
    void drawVertexList();
    void drawToMesh(Mesh *m, RenderState *s);
    VertexGroup * drawFaceToMeshCopy(RenderState *s, FaceGL1 f);
    void saveObjIndicesTri(FILE *f, FaceGL1 face);
    void saveObjIndicesQuad(FILE *f, FaceGL1 face);
    void saveObjIndicesTriStrip(FILE *f, FaceGL1 face);
    void saveObjFace(FILE *f, uint ind1, uint ind2, uint ind3, bool normals, bool texCoords);
    void saveObjIndice(FILE *f, uint indice, bool normals, bool texCoords);

    QVector<vec3> m_vertices;
    QVector<vec3> m_normals;
    QVector<vec2> m_texCoords;
    QVector<uint> m_indices;
    QVector<FaceGL1> m_faces;
};

#endif
