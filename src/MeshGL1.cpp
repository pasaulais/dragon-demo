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

int MeshGL1::groupCount() const
{
    return m_faces.count();
}

uint MeshGL1::groupMode(int index) const
{
    return m_faces.value(index).mode;
}

uint MeshGL1::groupSize(int index) const
{
    return m_faces.value(index).count;
}

void MeshGL1::addGroup(VertexGroup *vg)
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

bool MeshGL1::copyGroupTo(int index, VertexGroup *vg) const
{
    if((index < 0) || (index >= groupCount()))
        return false;
    FaceGL1 face = m_faces[index];
    if((uint)face.count > vg->count)
        return false;
    VertexData *d = vg->data;
    int endOffset = face.offset + face.count;
    for(int i = face.offset; i < endOffset; i++, d++)
    {
        d->position = m_vertices.value(i);
        d->normal = m_normals.value(i);
        d->texCoords = m_texCoords.value(i);
    }
    return true;
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
        out->addGroup(vg);
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
