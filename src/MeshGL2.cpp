#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <cmath>
#include <cstring>
#include "MeshGL2.h"
#include "Material.h"
#include "RenderState.h"
#include "RenderStateGL2.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

MeshGL2::MeshGL2(const RenderStateGL2 *state, QObject *parent) : Mesh(parent)
{
    m_state = state;
}

MeshGL2::~MeshGL2()
{
    foreach(VertexGroup *vg, m_groups)
    {
        if(vg->id != 0)
            glDeleteBuffersARB(1, &vg->id);
        delete vg;
    }
    m_groups.clear();
}

int MeshGL2::groupCount() const
{
    return m_groups.count();
}

uint MeshGL2::groupMode(int index) const
{
    VertexGroup *vg = m_groups.value(index);
    return vg ? vg->mode : 0;
}

uint MeshGL2::groupSize(int index) const
{
    VertexGroup *vg = m_groups.value(index);
    return vg ? vg->count : 0;
}

void MeshGL2::addGroup(VertexGroup *vg)
{
    VertexGroup *copy = new VertexGroup(vg->mode, vg->count);
    uint32_t size = vg->count * sizeof(VertexData);
    memcpy(copy->data, vg->data, size);
    m_groups.append(copy);
}

bool MeshGL2::copyGroupTo(int index, VertexGroup *vg) const
{
    if((index < 0) || (index >= groupCount()))
        return false;
    VertexGroup *source = m_groups[index];
    if(source->count > vg->count)
        return false;
    memcpy(vg->data, source->data, source->count * sizeof(VertexData));
    return true;
}

void MeshGL2::allocVBO(VertexGroup *vg)
{
    uint32_t size = vg->count * sizeof(VertexData);
    glGenBuffers(1, &vg->id);
    glBindBuffer(GL_ARRAY_BUFFER, vg->id);
    glBufferData(GL_ARRAY_BUFFER, size, vg->data, GL_STATIC_DRAW);
}

void MeshGL2::draw(Mesh::OutputMode mode, RenderState *s, Mesh *output)
{
    drawVBO();
}

void MeshGL2::drawVBO()
{
    /*glEnableVertexAttribArray(RenderStateGL2::ATTR_POSITION);
    glEnableVertexAttribArray(RenderStateGL2::ATTR_NORMAL);
    glEnableVertexAttribArray(RenderStateGL2::ATTR_TEXCOORDS);
    foreach(VertexGroup *vg, m_groups)
    {
        if(!vg->id)
            allocVBO(vg);
        else
            glBindBuffer(GL_ARRAY_BUFFER, vg->id);
        glVertexAttribPointer(RenderStateGL2::ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,
            sizeof(VertexData), BUFFER_OFFSET(0));
        glVertexAttribPointer(RenderStateGL2::ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
            sizeof(VertexData), BUFFER_OFFSET(sizeof(vec3)));
        glVertexAttribPointer(RenderStateGL2::ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE,
            sizeof(VertexData), BUFFER_OFFSET(2 * sizeof(vec3)));
        glDrawArrays(vg->mode, 0, vg->count);
    }
    glDisableVertexAttribArray(RenderStateGL2::ATTR_POSITION);
    glDisableVertexAttribArray(RenderStateGL2::ATTR_NORMAL);
    glDisableVertexAttribArray(RenderStateGL2::ATTR_TEXCOORDS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    */
    int position = m_state->positionAttr();
    int normal = m_state->normalAttr();
    int texCoords = m_state->texCoordsAttr();
    glEnableVertexAttribArray(position);
    glEnableVertexAttribArray(normal);
    glEnableVertexAttribArray(texCoords);
    foreach(VertexGroup *vg, m_groups)
    {
        glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE,
            sizeof(VertexData), &vg->data->position);
        glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE,
            sizeof(VertexData), &vg->data->normal);
        glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE,
            sizeof(VertexData), &vg->data->texCoords);
        glDrawArrays(vg->mode, 0, vg->count);
    }
    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(normal);
    glDisableVertexAttribArray(texCoords);
}
