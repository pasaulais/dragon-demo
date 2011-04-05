#ifndef INITIALS_MESH_H
#define INITIALS_MESH_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <cstdio>

bool fequal(double a, double b);

typedef struct
{
    uint mode;
    int count;
    int offset;   // location of the face vertices in the mesh's indices array
    bool draw;
} Face;

typedef struct
{
    QVector3D vertex;
    QVector3D normal;
    QVector2D texCoords;
} Point;

class Mesh : public QObject
{
public:
    Mesh(QObject *parent = 0);
    virtual ~Mesh();

    QVector<QVector3D> & vertices();
    const QVector<QVector3D> & vertices() const;
    void setVertices(float *vertices, int n);

    QVector<QVector3D> & normals();
    const QVector<QVector3D> & normals() const;
    void setNormals(float *normals, int n);

    QVector<QVector2D> & texCoords();
    const QVector<QVector2D> & texCoords() const;
    void setTexCoords(float *texCoords, int n);

    QVector<uint> & indices();
    const QVector<uint> & indices() const;
    void setIndices(uint *indices, int n);

    const QVector<Face> & faces() const;

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

    void draw(OutputMode mode, Mesh *output = 0);
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    void drawNormals();

    static Mesh * loadStl(const char *path, QObject *parent = 0, bool compute_normals = true);
    static Mesh * loadObj(const char *path, QObject *parent = 0);
    void saveStl(QString path);
    void saveObj(QString path);

private:
    // draw the mesh using glBegin/glEnd/glVertex/etc
    void drawImmediate();
    // draw the mesh using vertex lists, which is faster than calling glBegin/glEnd
    void drawVertexList();
    void drawToMesh(Mesh *m);
    void drawFaceToMeshCopy(Mesh *out, uint offset, Face f);
    void drawFaceToMeshQuad(Mesh *out, uint offset, Face f);
    void drawFaceToMeshTriStrip(Mesh *out, uint offset, Face f);
    static QMatrix4x4 currentGLMatrix();
    static QMatrix4x4 currentGLMatrixForNormals();
    void saveObjIndicesCopy(FILE *f, Face face);
    void saveObjIndicesQuad(FILE *f, Face face);
    void saveObjIndicesTriStrip(FILE *f, Face face);
    void saveObjFace(FILE *f, uint ind1, uint ind2, uint ind3, bool normals, bool texCoords);
    void saveObjIndice(FILE *f, uint indice, bool normals, bool texCoords);

    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_normals;
    QVector<QVector2D> m_texCoords;
    QVector<uint> m_indices;
    QVector<Face> m_faces;
};

#endif
