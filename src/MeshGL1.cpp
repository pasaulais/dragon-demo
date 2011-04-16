#include <cmath>
#include "Platform.h"
#include "MeshGL1.h"
#include "Material.h"
#include "RenderState.h"

MeshGL1::MeshGL1() : Mesh()
{
}

MeshGL1::~MeshGL1()
{
}

int MeshGL1::groupCount() const
{
    return m_faces.size();
}

uint32_t MeshGL1::groupMode(int index) const
{
    if((index < 0) || ((uint32_t)index > groupCount()))
        return 0;
    else
        return m_faces[index].mode;
}

uint32_t MeshGL1::groupSize(int index) const
{
    if((index < 0) || ((uint32_t)index > groupCount()))
        return 0;
    else
        return m_faces[index].count;
}

void MeshGL1::addGroup(VertexGroup *vg)
{
    uint32_t destOffset = m_vertices.size();
    uint32_t newSize = destOffset + vg->count;
    m_vertices.resize(newSize);
    m_normals.resize(newSize);
    m_texCoords.resize(newSize);

    VertexData *v = vg->data;
    for(uint32_t i = destOffset; i < newSize; i++, v++)
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
    Face face = m_faces[index];
    if((uint32_t)face.count > vg->count)
        return false;
    bool normals = m_normals.size() > 0;
    bool texCoords = m_texCoords.size() > 0;
    VertexData *d = vg->data;
    int endOffset = face.offset + face.count;
    for(int i = face.offset; i < endOffset; i++, d++)
    {
        d->position = m_vertices[i];
        d->normal = normals ? m_normals[i] : vec3();
        d->texCoords = texCoords ? m_texCoords[i] : vec2();
    }
    return true;
}

void MeshGL1::addFace(uint32_t mode, int vertexCount, int offset, bool draw)
{
    Face f;
    f.mode = mode;
    f.count = vertexCount;
    f.offset = offset;
    f.draw = draw;
    m_faces.push_back(f);
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
    bool normals = m_normals.size() > 0;
    bool texCoords = m_texCoords.size() > 0;
    if(normals)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, &m_normals[0]);
    }
    if(texCoords)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, &m_texCoords[0]);
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &m_vertices[0]);
    for(uint32_t i = 0; i < m_faces.size(); i++)
    {
        Face f = m_faces[i];
        if(f.draw)
            glDrawArrays(f.mode, f.offset, f.count);
    }
    if(texCoords)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if(normals)
        glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void MeshGL1::drawToMesh(Mesh *out, RenderState *s)
{
    if(!out || !s)
        return;
    for(uint32_t i = 0; i < m_faces.size(); i++)
    {
        Face f = m_faces[i];
        if(!f.draw)
            continue;
        VertexGroup *vg = drawFaceToMeshCopy(s, f);
        out->addGroup(vg);
        delete vg;
    }
}

VertexGroup * MeshGL1::drawFaceToMeshCopy(RenderState *s, Face f)
{
    bool normals = m_normals.size() > 0;
    bool texCoords = m_texCoords.size() > 0;
    VertexGroup *vg = new VertexGroup(f.mode, f.count);
    VertexData *v = vg->data;
    matrix4 m = s->currentMatrix();
    int endOffset = f.offset + f.count;
    for(int i = f.offset; i < endOffset; i++, v++)
    {
        v->position = m.map(m_vertices[i]);
        v->normal = normals ? m.mapNormal(m_normals[i]) : vec3();
        v->texCoords = texCoords ? m_texCoords[i] : vec2();
    }
    return vg;
}

/* Show the normal for every vertex in the mesh, for debugging purposes. */
void MeshGL1::drawNormals(RenderState *s)
{
#ifndef JNI_WRAPPER
    static Material mat(vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 0.0);
    if(m_normals.size() == 0)
        return;
    s->pushMaterial(mat);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    for(uint32_t i = 0; i < m_faces.size(); i++)
    {
        Face face = m_faces[i];
        if(!face.draw)
            continue;
        for(int p = 0; p < face.count; p++)
        {
            uint32_t ind = face.offset + p;
            vec3 v = m_vertices[ind];
            vec3 v2 = m_normals[ind] + v;
            glVertex3fv((GLfloat *)&v);
            glVertex3fv((GLfloat *)&v2);
        }
    }
    glEnd();
    glLineWidth(1.0);
    s->popMaterial();
#endif
}
