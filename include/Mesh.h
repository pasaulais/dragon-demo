#ifndef INITIALS_MESH_H
#define INITIALS_MESH_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

bool fequal(double a, double b);

typedef struct
{
    uint mode;
    int count;
    int offset;   // location of the face vertices in the mesh's indices array
    bool draw;
} Face;

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

    void draw();
    // draw the mesh using glBegin/glEnd/glVertex/etc
    void draw_immediate();
    // draw the mesh using vertex lists, which is faster than calling glBegin/glEnd
    void draw_vertex_list();

    // Show normal vectors for every vertex in the mesh, for debugging purposes
    void draw_normals();

    static Mesh * load_stl(const char *path, QObject *parent, bool compute_normals = true);

private:
    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_normals;
    QVector<QVector2D> m_texCoords;
    QVector<uint> m_indices;
    QVector<Face> m_faces;
    bool m_use_vertex_list;
};

#endif
