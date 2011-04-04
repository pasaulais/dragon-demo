#ifndef INITIALS_MESH_H
#define INITIALS_MESH_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <GL/gl.h>

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

    QVector3D *vertices() const;
    void setVertices(GLfloat *vertices, int n);
    QVector3D *allocVertices(int n);
    QVector3D *normals() const;
    void setNormals(GLfloat *normals, int n);
    QVector3D *allocNormals(int n);
    QVector2D *texCoords() const;
    void setTexCoords(GLfloat *texCoords, int n);
    uint *indices() const;
    void setIndices(GLuint *indices, int n);

    const QVector<Face> & faces() const;

    void addFace(uint mode, int vertexCount, int offset, bool draw = true);

    // Generate texture coordinates for 4 vertices-faced meshes
    void generate_quadri_textcoords(int indiceCount);

    void computeNormals(int indiceCount);

    void draw();
    // draw the mesh using glBegin/glEnd/glVertex/etc
    void draw_immediate();
    // draw the mesh using vertex lists, which is faster than calling glBegin/glEnd
    void draw_vertex_list();

    // Show normal vectors for every vertex in the mesh, for debugging purposes
    void draw_normals();

    static Mesh * load_stl(const char *path, QObject *parent, bool compute_normals = true);

private:
    // Compute the normal vector of the triangle ABC
    static void normal_vector(const QVector3D &a, const QVector3D &b, const QVector3D &c, QVector3D &n);

    QVector3D *m_vertices;
    QVector3D *m_normals;
    QVector2D *m_texCoords;
    uint *m_indices;
    QVector<Face> m_faces;
    bool m_use_vertex_list;
};

class Material
{
public:
    Material();
    Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine);
    void beginApply();
    void endApply();

    void setAmbient(const QVector4D &ambient);

private:
    QVector4D m_ambient;
    QVector4D m_diffuse;
    QVector4D m_specular;
    float m_shine;
    uint m_texture;
    bool m_useMipmaps;
};

#endif
