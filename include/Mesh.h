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

#ifndef INITIALS_MESH_H
#define INITIALS_MESH_H

#include <cstdio>
#include <string>
#include <iostream>
#include <inttypes.h>
#include "Vertex.h"

using namespace std;

class RenderState;

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    virtual int groupCount() const = 0;
    virtual uint32_t groupMode(int index) const = 0;
    virtual uint32_t groupSize(int index) const = 0;
    virtual void addGroup(VertexGroup *vg) = 0;
    virtual bool copyGroupTo(int index, VertexGroup *vg) const = 0;

    enum OutputMode
    {
        RenderToScreen,
        RenderToMesh
    };

    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0) = 0;
    // Show normal vectors for every vertex in the mesh, for debugging purposes
    virtual void drawNormals(RenderState *s);
    virtual void saveStl(string path) const;
    virtual void saveObj(string path) const;

    static VertexGroup * loadStl(string path, bool computeNormals = false);
    static VertexGroup * loadObj(string path);
    static VertexGroup * loadObj(istream &s);
    static void saveStl(string path, VertexGroup **vg, int groups);
    static void saveObj(string path, VertexGroup **vg, int groups);

private:
    static void saveObjIndicesTri(FILE *f, VertexGroup *vg, uint32_t &offset);
    static void saveObjIndicesQuad(FILE *f, VertexGroup *vg, uint32_t &offset);
    static void saveObjIndicesTriStrip(FILE *f, VertexGroup *vg, uint32_t &offset);
    static void saveObjFace(FILE *f, uint32_t ind1, uint32_t ind2, uint32_t ind3);
    static void saveObjIndice(FILE *f, uint32_t indice);
};

#endif
