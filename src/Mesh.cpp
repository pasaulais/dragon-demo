#include <GL/gl.h>
#include <cmath>
#include <cstdio>
#include "Mesh.h"

bool fequal(double a, double b)
{
    return fabs(a - b) < 1e-16;
}

Mesh::Mesh(QObject *parent) : QObject(parent)
{
    m_use_vertex_list = true;
}

Mesh::~Mesh()
{
}

QVector<QVector3D> & Mesh::vertices()
{
    return m_vertices;
}

const QVector<QVector3D> & Mesh::vertices() const
{
    return m_vertices;
}

void Mesh::setVertices(GLfloat *vertices, int n)
{
    if(!vertices)
        return;
    m_vertices.resize(n);
    for(int i = 0; i < n; i++, vertices += 3)
        m_vertices[i] = QVector3D(vertices[0], vertices[1], vertices[2]);
}

QVector<QVector3D> & Mesh::normals()
{
    return m_normals;
}

const QVector<QVector3D> & Mesh::normals() const
{
    return m_normals;
}

void Mesh::setNormals(GLfloat *normals, int n)
{
    if(!normals)
        return;
    m_normals.resize(n);
    for(int i = 0; i < n; i++, normals += 3)
        m_normals[i] = QVector3D(normals[0], normals[1], normals[2]);
}

QVector<QVector2D> & Mesh::texCoords()
{
    return m_texCoords;
}

const QVector<QVector2D> & Mesh::texCoords() const
{
    return m_texCoords;
}

void Mesh::setTexCoords(GLfloat *texCoords, int n)
{
    if(!texCoords)
        return;
    m_texCoords.resize(n);
    for(int i = 0; i < n; i++, texCoords += 2)
        m_texCoords[i] = QVector2D(texCoords[0], texCoords[1]);
}

QVector<uint> & Mesh::indices()
{
    return m_indices;
}

const QVector<uint> & Mesh::indices() const
{
    return m_indices;
}

void Mesh::setIndices(GLuint *indices, int n)
{
    if(!indices)
        return;
    m_indices.resize(n);
    for(int i = 0; i < n; i++, indices++)
        m_indices[i] = indices[0];
}

const QVector<Face> & Mesh::faces() const
{
    return m_faces;
}

void Mesh::addFace(uint mode, int vertexCount, int offset, bool draw)
{
    Face f;
    f.mode = mode;
    f.count = vertexCount;
    f.offset = offset;
    f.draw = draw;
    m_faces.append(f);
}

/* Generate texture coordinates for 4 vertices-faced meshes */
void Mesh::generate_quadri_textcoords(int indiceCount)
{
    m_texCoords.resize(indiceCount);
    foreach(Face f, m_faces)
    {
        for(GLuint j = 0; j < 4; j++)
        {
            GLuint ind = m_indices[f.offset + j];
            QVector2D *tc = &m_texCoords[ind];
            tc->setX(((j == 0) || (j == 3)) ? 0.0 : 1.0);
            tc->setY(((j == 0) || (j == 1)) ? 0.0 : 1.0);
        }
    }
}

void Mesh::computeNormals()
{
    int indiceCount = m_indices.count();
    if((indiceCount == 0) || (m_vertices.count() < indiceCount))
        return;

    QVector3D u;
    m_normals.resize(indiceCount);
    foreach(Face f, m_faces)
    {
        // use the face's first three vertices to compute its normal vector
        if(f.count < 3)
            continue;
        GLuint ind1 = m_indices[f.offset + 0];
        GLuint ind2 = m_indices[f.offset + 1];
        GLuint ind3 = m_indices[f.offset + 2];
        u = QVector3D::normal(m_vertices[ind1], m_vertices[ind2], m_vertices[ind3]);

        // assign it to every vertex in the face
        for(int i = 0; i < f.count; i++)
            m_normals[f.offset + i] = u;
    }
}

void Mesh::draw()
{
    if(m_use_vertex_list)
        draw_vertex_list();
    else
        draw_immediate();
}

// draw the mesh using glBegin/glEnd/glVertex/etc
void Mesh::draw_immediate()
{
    foreach(Face f, m_faces)
    {
        if(f.draw)
        {
            glBegin(f.mode);
            for(GLsizei j = 0; j < f.count; j++)
            {
                GLuint ind = (m_indices.count() > 0) ? m_indices[f.offset + j] : f.offset + j;
                if(m_normals.count() > 0)
                    glNormal3fv((GLfloat *)&m_normals[ind]);
                if(m_texCoords.count() > 0)
                    glTexCoord2fv((GLfloat *)&m_texCoords[ind]);
                glVertex3fv((GLfloat *)&m_vertices[ind]);
            }
            glEnd();
        }
    }
}

// draw the mesh using vertex lists, which is faster than calling glBegin/glEnd
void Mesh::draw_vertex_list()
{
    if(m_normals.count() > 0)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, m_normals.data());
    }
    if(m_texCoords.count() > 0)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords.data());
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_vertices.data());
    foreach(Face f, m_faces)
    {
        if(f.draw)
        {
            if(m_indices.count() > 0)
                glDrawElements(f.mode, f.count, GL_UNSIGNED_INT, (m_indices.data() + f.offset));
            else
                glDrawArrays(f.mode, f.offset, f.count);
        }
    }
    if(m_texCoords.count() > 0)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if(m_normals.count() > 0)
        glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/* Show the normal for every vertex in the mesh, for debugging purposes. */
void Mesh::draw_normals()
{
    static Material mat(QVector4D(1.0, 1.0, 1.0, 1.0),
        QVector4D(0.0, 0.0, 0.0, 1.0), QVector4D(0.0, 0.0, 0.0, 1.0), 0.0);
    if(m_normals.count() == 0)
        return;
    mat.beginApply();
    glLineWidth(3.0);
    glBegin(GL_LINES);
    foreach(Face face, m_faces)
    {
        if(!face.draw)
            continue;
        for(int p = 0; p < face.count; p++)
        {
            uint ind = (m_indices.count() > 0) ? m_indices[face.offset + p] : face.offset + p;
            QVector3D v = m_vertices[ind];
            QVector3D v2 = m_normals[ind] + v;
            glVertex3fv((GLfloat *)&v);
            glVertex3fv((GLfloat *)&v2);
        }
    }
    glEnd();
    glLineWidth(1.0);
    mat.endApply();
}

Mesh * Mesh::load_stl(const char *path, QObject *parent, bool compute_normals)
{
    Mesh *m = 0;
    FILE *f = fopen(path, "rb");
    char header[80];
    uint32_t triangles = 0;
    QVector3D points[4];
    uint16_t attributes;

    // read file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s'.\n", path);
        return 0;
    }
    if((fread(header, sizeof(header), 1, f) < 1) ||
            (fread(&triangles, 4, 1, f) < 1))
    {
        fclose(f);
        return 0;
    }

    // read vertex data
    m = new Mesh(parent);
    QVector<QVector3D> &vertices = m->vertices();
    QVector<QVector3D> &normals = m->normals();
    vertices.resize(3 * triangles);
    normals.resize(3 * triangles);
    for(uint32_t i = 0; i < triangles; i++)
    {
        if((fread(points, sizeof(points), 1, f) < 1) ||
            (fread(&attributes, 2, 1, f) < 1))
        {
            delete m;
            fclose(f);
            return 0;
        }
        if(compute_normals)
            points[0] = QVector3D::normal(points[1], points[2], points[3]);
        for(uint32_t v = 0; v < 3; v++)
        {
            uint32_t pos = (i * 3) + v;
            vertices[pos] = points[v + 1];
            normals[pos] = points[0];
        }
    }
    m->addFace(GL_TRIANGLES, triangles * 3, 0);
    fclose(f);
    return m;
}

///////////////////////////////////////////////////////////////////////////////
Material::Material()
{
    m_ambient = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_diffuse = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_specular = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_shine = 0.0;
    m_texture = 0;
}

Material::Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shine = shine;
}

void Material::setAmbient(const QVector4D &ambient)
{
    m_ambient = ambient;
}

void Material::beginApply()
{
    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat *)&m_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat *)&m_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat *)&m_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, m_shine);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    if(m_useMipmaps)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Material::endApply()
{
    glPopAttrib();
}
