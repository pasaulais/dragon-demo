#ifndef INITIALS_MESH_GL2_H
#define INITIALS_MESH_GL2_H

#include <QObject>
#include <QVector>
#include "Vertex.h"
#include <cstdio>

class RenderState;

typedef struct
{
    uint mode;
    int count;
    int offset;   // location of the face vertices in the mesh's indices array
    bool draw;
} FaceGL2;

class MeshGL2 : public QObject
{
public:
    MeshGL2(QObject *parent = 0);
    virtual ~MeshGL2();

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

    const QVector<FaceGL2> & faces() const;

    void addFace(uint mode, int vertexCount, int offset, bool draw = true);

    // Generate texture coordinates for 4 vertices-faced meshes
    void generateTextureCoords();

    void computeNormals();

    enum OutputMode
    {
        Output_Immediate,
        Output_VertexList,
        Output_Mesh
    };

    void draw(OutputMode mode, RenderState *s, MeshGL2 *output = 0);
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    void drawNormals(RenderState *s);

    static MeshGL2 * loadStl(const char *path, QObject *parent = 0, bool compute_normals = true);
    static MeshGL2 * loadObj(const char *path, QObject *parent = 0);
    void saveStl(QString path);
    void saveObj(QString path);

private:
    // draw the mesh using glBegin/glEnd/glVertex/etc
    void drawImmediate();
    // draw the mesh using vertex lists, which is faster than calling glBegin/glEnd
    void drawVertexList();
    void drawToMesh(MeshGL2 *m, RenderState *s);
    void drawFaceToMeshCopy(MeshGL2 *out, RenderState *s, uint offset, FaceGL2 f);
    void saveObjIndicesTri(FILE *f, FaceGL2 face);
    void saveObjIndicesQuad(FILE *f, FaceGL2 face);
    void saveObjIndicesTriStrip(FILE *f, FaceGL2 face);
    void saveObjFace(FILE *f, uint ind1, uint ind2, uint ind3, bool normals, bool texCoords);
    void saveObjIndice(FILE *f, uint indice, bool normals, bool texCoords);

    QVector<vec3> m_vertices;
    QVector<vec3> m_normals;
    QVector<vec2> m_texCoords;
    QVector<uint> m_indices;
    QVector<FaceGL2> m_faces;
};

#endif
