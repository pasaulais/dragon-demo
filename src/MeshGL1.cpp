#include <GL/gl.h>
#include <cmath>
#include <cstdio>
#include "MeshGL1.h"
#include "Material.h"
#include "RenderState.h"

MeshGL1::MeshGL1(QObject *parent) : Mesh(parent)
{
}

MeshGL1::~MeshGL1()
{
}

QVector<vec3> & MeshGL1::vertices()
{
    return m_vertices;
}

const QVector<vec3> & MeshGL1::vertices() const
{
    return m_vertices;
}

void MeshGL1::setVertices(GLfloat *vertices, int n)
{
    if(!vertices)
        return;
    m_vertices.resize(n);
    for(int i = 0; i < n; i++, vertices += 3)
        m_vertices[i] = vec3(vertices[0], vertices[1], vertices[2]);
}

QVector<vec3> & MeshGL1::normals()
{
    return m_normals;
}

const QVector<vec3> & MeshGL1::normals() const
{
    return m_normals;
}

void MeshGL1::setNormals(float *normals, int n)
{
    if(!normals)
        return;
    m_normals.resize(n);
    for(int i = 0; i < n; i++, normals += 3)
        m_normals[i] = vec3(normals[0], normals[1], normals[2]);
}

QVector<vec2> & MeshGL1::texCoords()
{
    return m_texCoords;
}

const QVector<vec2> & MeshGL1::texCoords() const
{
    return m_texCoords;
}

void MeshGL1::setTexCoords(float *texCoords, int n)
{
    if(!texCoords)
        return;
    m_texCoords.resize(n);
    for(int i = 0; i < n; i++, texCoords += 2)
        m_texCoords[i] = vec2(texCoords[0], texCoords[1]);
}

const QVector<FaceGL1> & MeshGL1::faces() const
{
    return m_faces;
}

void MeshGL1::addVertices(VertexGroup *vg)
{
    uint destOffset = m_vertices.count();
    uint newSize = destOffset + vg->count;
    m_vertices.resize(newSize);
    m_normals.resize(newSize);
    m_texCoords.resize(newSize);

    VertexData *v = vg->data;
    for(uint i = destOffset; i < newSize; i++, v++)
    {
        m_vertices[i] = v->position;
        m_normals[i] = v->normal;
        m_texCoords[i] = v->texCoords;
    }
    addFace(vg->mode, vg->count, destOffset);
}

void MeshGL1::addFace(uint mode, int vertexCount, int offset, bool draw)
{
    FaceGL1 f;
    f.mode = mode;
    f.count = vertexCount;
    f.offset = offset;
    f.draw = draw;
    m_faces.append(f);
}

void MeshGL1::draw(MeshGL1::OutputMode mode, RenderState *s, Mesh *output)
{
    switch(mode)
    {
    default:
    case RenderToScreen:
        drawVertexList();
        break;
    case RenderToMesh:
        drawToMesh(output, s);
        break;
    }
}

void MeshGL1::drawVertexList()
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
    foreach(FaceGL1 f, m_faces)
    {
        if(f.draw)
            glDrawArrays(f.mode, f.offset, f.count);
    }
    if(m_texCoords.count() > 0)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if(m_normals.count() > 0)
        glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void MeshGL1::drawToMesh(Mesh *out, RenderState *s)
{
    if(!out || !s)
        return;
    foreach(FaceGL1 f, m_faces)
    {
        if(!f.draw)
            continue;
        VertexGroup *vg = drawFaceToMeshCopy(s, f);
        out->addVertices(vg);
        delete vg;
    }
}

VertexGroup * MeshGL1::drawFaceToMeshCopy(RenderState *s, FaceGL1 f)
{
    VertexGroup *vg = new VertexGroup(f.mode, f.count);
    VertexData *v = vg->data;
    matrix4 m = s->currentMatrix();
    int endOffset = f.offset + f.count;
    for(int i = f.offset; i < endOffset; i++, v++)
    {
        v->position = m.map(m_vertices.value(i));
        v->normal = m.mapNormal(m_normals.value(i));
        v->texCoords = m_texCoords.value(i);
    }
    return vg;
}

/* Show the normal for every vertex in the mesh, for debugging purposes. */
void MeshGL1::drawNormals(RenderState *s)
{
    static Material mat(vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 0.0);
    if(m_normals.count() == 0)
        return;
    s->pushMaterial(mat);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    foreach(FaceGL1 face, m_faces)
    {
        if(!face.draw)
            continue;
        for(int p = 0; p < face.count; p++)
        {
            uint ind = face.offset + p;
            vec3 v = m_vertices[ind];
            vec3 v2 = m_normals[ind] + v;
            glVertex3fv((GLfloat *)&v);
            glVertex3fv((GLfloat *)&v2);
        }
    }
    glEnd();
    glLineWidth(1.0);
    s->popMaterial();
}

void MeshGL1::saveStl(QString path)
{
    FILE *f = fopen(path.toUtf8().data(), "wb");
    char header[80];
    uint32_t triangles = 0;
    uint16_t attributes = 0;

    // count triangles
    foreach(FaceGL1 f, m_faces)
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
    QVector<vec3> lastVertices;
    QVector<vec3> lastNormals;
    foreach(FaceGL1 face, m_faces)
    {
        if(face.mode != GL_TRIANGLES)
            continue;
        for(int i = 0; i < face.count; i++)
        {
            uint srcIndex = face.offset + i;
            lastVertices.append(m_vertices.value(srcIndex));
            lastNormals.append(m_normals.value(srcIndex));
            if(lastVertices.count() == 3)
            {
                vec3 normal = lastNormals[0];
                fwrite(&normal, 3 * sizeof(float), 1, f);
                foreach(vec3 vertex, lastVertices)
                    fwrite(&vertex, 3 * sizeof(float), 1, f);
                fwrite(&attributes, sizeof(attributes), 1, f);
                lastVertices.clear();
                lastNormals.clear();
            }
        }
    }
    fclose(f);
}

void MeshGL1::saveObj(QString path)
{
    FILE *f = fopen(path.toUtf8().data(), "w");

    // write file header
    if(f == 0)
    {
        fprintf(stderr, "Could not open file '%s' for writing.\n", path.toUtf8().data());
        return;
    }
    foreach(vec3 vertex, m_vertices)
        fprintf(f, "v %f %f %f\n", vertex.x, vertex.y, vertex.z);
    foreach(vec3 normal, m_normals)
        fprintf(f, "vn %f %f %f\n", normal.x, normal.y, normal.z);
    foreach(vec2 texCoords, m_texCoords)
        fprintf(f, "vt %f %f\n", texCoords.x, texCoords.y);
    foreach(FaceGL1 face, m_faces)
    {
        switch(face.mode)
        {
        case GL_TRIANGLES:
            saveObjIndicesTri(f, face);
            break;
        case GL_QUADS:
            saveObjIndicesQuad(f, face);
            break;
        case GL_TRIANGLE_STRIP:
            saveObjIndicesTriStrip(f, face);
            break;
        }
    }
    fclose(f);
}

void MeshGL1::saveObjIndicesTri(FILE *f, FaceGL1 face)
{
    bool normals = m_normals.count() > 0;
    bool texCoords = m_texCoords.count() > 0;
    int endOffset = face.offset + face.count;
    for(int i = face.offset; i < endOffset; i += 3)
        saveObjFace(f, i, i + 1, i + 2, normals, texCoords);
}

void MeshGL1::saveObjIndicesQuad(FILE *f, FaceGL1 face)
{
    bool normals = m_normals.count() > 0;
    bool texCoords = m_texCoords.count() > 0;
    QVector<uint> indices;
    int endOffset = face.offset + face.count;
    for(int i = face.offset; i < endOffset; i += 4)
    {
        saveObjFace(f, i, i + 1, i + 2, normals, texCoords);
        saveObjFace(f, i, i + 2, i + 3, normals, texCoords);
    }
}

void MeshGL1::saveObjIndicesTriStrip(FILE *f, FaceGL1 face)
{
    bool normals = m_normals.count() > 0;
    bool texCoords = m_texCoords.count() > 0;
    int endOffset = face.offset + face.count - 2;
    for(int i = face.offset; i < endOffset; i++)
        saveObjFace(f, i, i + 1, i + 2, normals, texCoords);
}

void MeshGL1::saveObjFace(FILE *f, uint ind1, uint ind2, uint ind3, bool normals, bool texCoords)
{
    fprintf(f, "f ");
    saveObjIndice(f, ind1 + 1, normals, texCoords);
    fprintf(f, " ");
    saveObjIndice(f, ind2 + 1, normals, texCoords);
    fprintf(f, " ");
    saveObjIndice(f, ind3 + 1, normals, texCoords);
    fprintf(f, "\n");
}

void MeshGL1::saveObjIndice(FILE *f, uint ind, bool normals, bool texCoords)
{
    if(texCoords && normals)
        fprintf(f, "%d/%d/%d", ind, ind, ind);
    else if(normals)
        fprintf(f, "%d//%d", ind, ind);
    else if(texCoords)
        fprintf(f, "%d/%d", ind, ind);
    else
        fprintf(f, "%d", ind);
}
