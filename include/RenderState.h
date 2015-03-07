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

#ifndef INITIALS_RENDER_STATE_H
#define INITIALS_RENDER_STATE_H

#include <map>
#include <string>
#include "Mesh.h"
#include "Material.h"
#include "Vertex.h"

using namespace std;

class RenderState
{
public:
    RenderState();
    virtual ~RenderState();

    virtual void init();

    virtual bool drawNormals() const;
    virtual void toggleNormals();
    virtual void toggleWireframe();
    virtual void toggleProjection();

    virtual void reset();

    // mesh operations
    virtual void drawMesh(Mesh *m) = 0;
    virtual void drawMesh(string name);

    virtual void beginExportMesh(string path);
    virtual void endExportMesh();

    virtual map<string, Mesh *> & meshes();
    virtual const map<string, Mesh *> & meshes() const;

    virtual Mesh * createMesh() const = 0;
    virtual Mesh * loadMeshFromFile(string name, string path);
    virtual Mesh * loadMeshFromData(string name, const char *data, size_t size);
    virtual Mesh * loadMeshFromGroup(string name, VertexGroup *vg);
    virtual void freeMeshes();

    virtual uint32_t loadTextureFromFile(string name, string path, bool mipmaps = false);
    virtual uint32_t loadTextureFromData(string name, const char *data, size_t size, bool mipmaps = false);
    virtual uint32_t texture(string name) const;
    virtual void freeTextures() = 0;

    // matrix operations

    enum MatrixMode
    {
        ModelView,
        Projection,
        Texture
    };

    virtual void setMatrixMode(MatrixMode newMode) = 0;

    virtual void loadIdentity() = 0;
    virtual void multiplyMatrix(const matrix4 &m) = 0;
    virtual void pushMatrix() = 0;
    virtual void popMatrix() = 0;

    virtual void translate(float dx, float dy, float dz) = 0;
    virtual void rotate(float angle, float rx, float ry, float rz) = 0;
    virtual void scale(float sx, float sy, float sz) = 0;

    virtual matrix4 currentMatrix() const = 0;

    // general state operations
    virtual void beginFrame(int width, int heigth) = 0;
    virtual void setupViewport(int width, int heigth) = 0;
    virtual void endFrame() = 0;

    // material operations

    virtual void pushMaterial(const Material &m) = 0;
    virtual void popMaterial() = 0;

protected:
    Mesh::OutputMode m_output;
    bool m_drawNormals;
    bool m_projection;
    bool m_wireframe;
    vec4 m_bgColor;
    Mesh *m_meshOutput;
    map<string, uint32_t> m_textures;
    map<string, Mesh *> m_meshes;

    // exporting
    bool m_exporting;
    string m_exportPath;
    Mesh::OutputMode m_oldOutput;
};

class StateObject
{
public:
    StateObject(RenderState *s);

    void loadIdentity();
    void pushMatrix();
    void popMatrix();

    void translate(float dx, float dy, float dz);
    void rotate(float angle, float rx, float ry, float rz);
    void scale(float sx, float sy, float sz);

    void drawMesh(Mesh *m);
    void drawMesh(string name);

    void pushMaterial(const Material &m);
    void popMaterial();

protected:
    RenderState *m_state;
};

#endif
