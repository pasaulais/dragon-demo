// Copyright (c) 2009-2015, Pierre-Andre Saulais <pasaulais@free.fr>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <cmath>
#include <cstring>
#include "Platform.h"
#include "MeshGL2.h"
#include "Material.h"
#include "RenderState.h"
#include "RenderStateGL2.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

MeshGL2::MeshGL2(const RenderStateGL2 *state) : Mesh()
{
    m_state = state;
}

MeshGL2::~MeshGL2()
{
    for(uint32_t i = 0; i < m_groups.size(); i++)
    {
        VertexGroup *vg = m_groups[i];
        if(vg->id != 0)
            glDeleteBuffers(1, &vg->id);
        delete vg;
    }
    m_groups.clear();
}

int MeshGL2::groupCount() const
{
    return m_groups.size();
}

uint32_t MeshGL2::groupMode(int index) const
{
    if((index < 0) || ((uint32_t)index > groupCount()))
        return 0;
    else
        return m_groups[index]->mode;
}

uint32_t MeshGL2::groupSize(int index) const
{
    if((index < 0) || ((uint32_t)index > groupCount()))
        return 0;
    else
        return m_groups[index]->count;
}

void MeshGL2::addGroup(VertexGroup *vg)
{
    VertexGroup *copy = new VertexGroup(vg->mode, vg->count);
    uint32_t size = vg->count * sizeof(VertexData);
    memcpy(copy->data, vg->data, size);
    m_groups.push_back(copy);
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

void MeshGL2::draw(Mesh::OutputMode mode, RenderState *s, Mesh *output)
{
    (void)mode;
    (void)s;
    (void)output;
    drawToScreen();
}

void MeshGL2::drawToScreen()
{
    int position = m_state->positionAttr();
    int normal = m_state->normalAttr();
    int texCoords = m_state->texCoordsAttr();
    glEnableVertexAttribArray(position);
    glEnableVertexAttribArray(normal);
    glEnableVertexAttribArray(texCoords);
    for(uint32_t i = 0; i < m_groups.size(); i++)
    {
        VertexGroup *vg = m_groups[i];
        if(vg->count > 100)
            drawVBO(vg, position, normal, texCoords);
        else
            drawArray(vg, position, normal, texCoords);
    }
    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(normal);
    glDisableVertexAttribArray(texCoords);
}

void MeshGL2::drawArray(VertexGroup *vg, int position, int normal, int texCoords)
{
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexData), &vg->data->position);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexData), &vg->data->normal);
    glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE,
        sizeof(VertexData), &vg->data->texCoords);
    glDrawArrays(vg->mode, 0, vg->count);
}

void MeshGL2::drawVBO(VertexGroup *vg, int position, int normal, int texCoords)
{
    if(vg->id == 0)
    {
        uint32_t size = vg->count * sizeof(VertexData);
        glGenBuffers(1, &vg->id);
        glBindBuffer(GL_ARRAY_BUFFER, vg->id);
        glBufferData(GL_ARRAY_BUFFER, size, vg->data, GL_STATIC_DRAW);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, vg->id);
    }
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexData), BUFFER_OFFSET(0));
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexData), BUFFER_OFFSET(sizeof(vec3)));
    glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE,
        sizeof(VertexData), BUFFER_OFFSET(2 * sizeof(vec3)));
    glDrawArrays(vg->mode, 0, vg->count);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
