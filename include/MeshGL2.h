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

#ifndef INITIALS_MESH_GL2_H
#define INITIALS_MESH_GL2_H

#include <vector>
#include <inttypes.h>
#include "Mesh.h"
#include "Vertex.h"

class RenderState;
class RenderStateGL2;

class MeshGL2 : public Mesh
{
public:
    MeshGL2(const RenderStateGL2 *state);
    virtual ~MeshGL2();

    virtual int groupCount() const;
    virtual uint32_t groupMode(int index) const;
    virtual uint32_t groupSize(int index) const;
    virtual void addGroup(VertexGroup *vg);
    virtual bool copyGroupTo(int index, VertexGroup *vg) const;
    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0);

private:
    void drawToScreen();
    void drawArray(VertexGroup *vg, int position, int normal, int texCoords);
    void drawVBO(VertexGroup *vg, int position, int normal, int texCoords);

    const RenderStateGL2 *m_state;
    std::vector<VertexGroup *> m_groups;
};

#endif
