#include <GL/gl.h>
#include <QMatrix4x4>
#include <cmath>
#include <cstdio>
#include "Mesh.h"
#include "Material.h"

bool fequal(double a, double b)
{
    return fabs(a - b) < 1e-16;
}

Mesh::Mesh(QObject *parent) : QObject(parent)
{
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

void Mesh::setNormals(float *normals, int n)
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

void Mesh::setTexCoords(float *texCoords, int n)
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

void Mesh::setIndices(uint *indices, int n)
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
void Mesh::generateTextureCoords()
{
    int indiceCount = m_indices.count();
    if((indiceCount == 0) || (m_vertices.count() < indiceCount))
        return;
    m_texCoords.resize(indiceCount);
    foreach(Face f, m_faces)
    {
        if(f.count != 4)
            return;
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

void Mesh::draw(Mesh::OutputMode mode, Mesh *output)
{
    switch(mode)
    {
    default:
    case Output_VertexList:
        drawVertexList();
        break;
    case Output_Immediate:
        drawImmediate();
        break;
    case Output_Mesh:
        drawToMesh(output);
        break;
    }
}

// draw the mesh using glBegin/glEnd/glVertex/etc
void Mesh::drawImmediate()
{
    foreach(Face f, m_faces)
    {
        if(f.draw)
        {
            glBegin(f.mode);
            for(int j = 0; j < f.count; j++)
            {
                uint ind = (m_indices.count() > 0) ? m_indices[f.offset + j] : f.offset + j;
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
void Mesh::drawVertexList()
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

void Mesh::drawToMesh(Mesh *out)
{
    if(!out)
        return;
    // determine the end of the indices buffer
    uint maxEnd = 0;
    foreach(Face f, out->faces())
    {
        uint faceEnd = f.offset + f.count;
        maxEnd = std::max(maxEnd, faceEnd);
    }

    // count the number of vertices in this mesh
    uint indiceCount = 0;
    foreach(Face f, m_faces)
    {
        switch(f.mode)
        {
        case GL_QUADS:
            indiceCount += ((f.count * 6) / 4);
            break;
        case GL_TRIANGLES:
            indiceCount += f.count;
            break;
        case GL_TRIANGLE_STRIP:
            indiceCount += ((f.count - 2) * 3);
            break;
        }
    }

    // allocate space for these vertices
    uint newCount = maxEnd + indiceCount;
    out->vertices().resize(newCount);
    out->normals().resize(newCount);
    out->texCoords().resize(newCount);
    out->indices().resize(newCount);
    uint destOffset = maxEnd;
    foreach(Face f, m_faces)
    {
        // convert the face to a list of triangles
        switch(f.mode)
        {
        case GL_QUADS:
            indiceCount = ((f.count * 6) / 4);
            drawFaceToMeshQuad(out, destOffset, f);
            break;
        case GL_TRIANGLES:
            indiceCount = f.count;
            drawFaceToMeshCopy(out, destOffset, f);
            break;
        case GL_TRIANGLE_STRIP:
            indiceCount = ((f.count - 2) * 3);
            drawFaceToMeshTriStrip(out, destOffset, f);
            break;
        }
        if(indiceCount > 0)
        {
            out->addFace(GL_TRIANGLES, indiceCount, destOffset);
            destOffset += indiceCount;
        }
    }
}

void Mesh::drawFaceToMeshCopy(Mesh *out, uint destOffset, Face f)
{
    QVector<uint> & outIndices = out->indices();
    QVector<QVector3D> & outVertices = out->vertices();
    QVector<QVector3D> & outNormals = out->normals();
    QVector<QVector2D> & outTexCoords = out->texCoords();
    QMatrix4x4 m = currentGLMatrix();
    for(int i = 0; i < f.count; i++, destOffset++)
    {
        uint srcIndex = m_indices.value(f.offset + i);
        outIndices[destOffset] = destOffset;
        outVertices[destOffset] = m.map(m_vertices.value(srcIndex));
        outNormals[destOffset] = m_normals.value(srcIndex);
        outTexCoords[destOffset] = m_texCoords.value(srcIndex);
    }
}

void Mesh::drawFaceToMeshQuad(Mesh *out, uint destOffset, Face f)
{
    QVector<uint> & outIndices = out->indices();
    QVector<QVector3D> & outVertices = out->vertices();
    QVector<QVector3D> & outNormals = out->normals();
    QVector<QVector2D> & outTexCoords = out->texCoords();
    QVector<QVector3D> lastVertices;
    QVector<QVector3D> lastNormals;
    QVector<QVector2D> lastTexCoords;
    QMatrix4x4 m = currentGLMatrix();
    for(int i = 0; i < f.count; i++)
    {
        uint srcIndex = m_indices.value(f.offset + i);
        lastVertices.append(m.map(m_vertices.value(srcIndex)));
        lastNormals.append(m_normals.value(srcIndex));
        lastTexCoords.append(m_texCoords.value(srcIndex));
        if(lastVertices.count() == 4)
        {
            static int Tri1[] = {0, 1, 2};
            for(int j = 0; j < 3; j++, destOffset++)
            {
                outIndices[destOffset] = destOffset;
                outVertices[destOffset] = lastVertices[Tri1[j]];
                outNormals[destOffset] = lastNormals[Tri1[j]];
                outTexCoords[destOffset] = lastTexCoords[Tri1[j]];
            }
            static int Tri2[] = {0, 2, 3};
            for(int j = 0; j < 3; j++, destOffset++)
            {
                outIndices[destOffset] = destOffset;
                outVertices[destOffset] = lastVertices[Tri2[j]];
                outNormals[destOffset] = lastNormals[Tri2[j]];
                outTexCoords[destOffset] = lastTexCoords[Tri2[j]];
            }
            lastVertices.clear();
            lastNormals.clear();
            lastTexCoords.clear();
        }
    }
}

void Mesh::drawFaceToMeshTriStrip(Mesh *out, uint destOffset, Face f)
{
    QVector<uint> & outIndices = out->indices();
    QVector<QVector3D> & outVertices = out->vertices();
    QVector<QVector3D> & outNormals = out->normals();
    QVector<QVector2D> & outTexCoords = out->texCoords();
    QVector<QVector3D> lastVertices;
    QVector<QVector3D> lastNormals;
    QVector<QVector2D> lastTexCoords;
    QMatrix4x4 m = currentGLMatrix();
    for(int i = 0; i < f.count; i++)
    {
        uint srcIndex = m_indices.value(f.offset + i);
        lastVertices.append(m.map(m_vertices.value(srcIndex)));
        lastNormals.append(m_normals.value(srcIndex));
        lastTexCoords.append(m_texCoords.value(srcIndex));
        if(i >= 2)
        {
            for(int j = 0; j < 3; j++, destOffset++)
            {
                outIndices[destOffset] = destOffset;
                outVertices[destOffset] = lastVertices[j];
                outNormals[destOffset] = lastNormals[j];
                outTexCoords[destOffset] = lastTexCoords[j];
            }
            lastVertices.remove(0);
            lastNormals.remove(0);
            lastTexCoords.remove(0);
        }
    }
}

QMatrix4x4 Mesh::currentGLMatrix()
{
    float m[4][4];
    glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);
    return QMatrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                     m[0][1], m[1][1], m[2][1], m[3][1],
                     m[0][2], m[1][2], m[2][2], m[3][2],
                     m[0][3], m[1][3], m[2][3], m[3][3]);
}

/* Show the normal for every vertex in the mesh, for debugging purposes. */
void Mesh::drawNormals()
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

Mesh * Mesh::loadStl(const char *path, QObject *parent, bool compute_normals)
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

void Mesh::saveStl(QString path)
{
    FILE *f = fopen(path.toUtf8().data(), "wb");
    char header[80];
    uint32_t triangles = 0;
    uint16_t attributes = 0;

    // count triangles
    foreach(Face f, m_faces)
    {
        if(f.mode == GL_TRIANGLES)
            triangles += (f.count / 3);
    }

    // write file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s' for writing.\n", path.toUtf8().data());
        return;
    }
    memset(header, 0, sizeof(header));
    fwrite(header, sizeof(header), 1, f);
    fwrite(&triangles, sizeof(uint32_t), 1, f);

    // write vertex data
    QVector<QVector3D> lastVertices;
    QVector<QVector3D> lastNormals;
    foreach(Face face, m_faces)
    {
        if(face.mode != GL_TRIANGLES)
            continue;
        for(int i = 0; i < face.count; i++)
        {
            uint srcIndex = m_indices.value(face.offset + i);
            lastVertices.append(m_vertices.value(srcIndex));
            lastNormals.append(m_normals.value(srcIndex));
            if(lastVertices.count() == 3)
            {
                QVector3D normal = lastNormals[0];
                fwrite(&normal, 3 * sizeof(float), 1, f);
                foreach(QVector3D vertex, lastVertices)
                    fwrite(&vertex, 3 * sizeof(float), 1, f);
                fwrite(&attributes, sizeof(attributes), 1, f);
                lastVertices.clear();
                lastNormals.clear();
            }
        }
    }
    fclose(f);
}
