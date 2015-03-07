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

#ifndef INITIALS_MESH_GL1_H
#define INITIALS_MESH_GL1_H

#include <vector>
#include <inttypes.h>
#include "Mesh.h"

typedef struct
{
    uint32_t mode;
    int count;
    int offset;   // location of the face vertices in the mesh's indices array
    bool draw;
} Face;

class MeshGL1 : public Mesh
{
public:
    MeshGL1();
    virtual ~MeshGL1();

    virtual int groupCount() const;
    virtual uint32_t groupMode(int index) const;
    virtual uint32_t groupSize(int index) const;
    virtual void addGroup(VertexGroup *vg);
    virtual bool copyGroupTo(int index, VertexGroup *vg) const;
    void addFace(uint32_t mode, int vertexCount, int offset, bool draw = true);

    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0);
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    virtual void drawNormals(RenderState *s);

private:
    void drawVertexList();
    void drawToMesh(Mesh *m, RenderState *s);
    VertexGroup * drawFaceToMeshCopy(RenderState *s, Face f);

    std::vector<vec3> m_vertices;
    std::vector<vec3> m_normals;
    std::vector<vec2> m_texCoords;
    std::vector<Face> m_faces;
};

#endif
