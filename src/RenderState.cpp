#include <sstream>
#include "RenderState.h"

RenderState::RenderState()
{
    m_meshOutput = 0;
    m_exporting = false;
    m_oldOutput = m_output;
    m_bgColor = vec4(0.6, 0.6, 1.0, 1.0);
    reset();
}

RenderState::~RenderState()
{
    freeMeshes();
}

map<string, Mesh *> & RenderState::meshes()
{
    return m_meshes;
}

const map<string, Mesh *> & RenderState::meshes() const
{
    return m_meshes;
}

Mesh * RenderState::loadMeshFromFile(string name, string path)
{
    return loadMeshFromGroup(name, Mesh::loadObj(path));
}

Mesh * RenderState::loadMeshFromData(string name, const char *data, size_t size)
{
    string contents(data, size);
    std::stringstream s(contents, ios_base::in);
    return loadMeshFromGroup(name, Mesh::loadObj(s));
}

void RenderState::freeMeshes()
{
    map<string, Mesh *>::iterator it;
    for(it = m_meshes.begin(); it != m_meshes.end(); it++)
        delete it->second;
    m_meshes.clear();
}

Mesh * RenderState::loadMeshFromGroup(string name, VertexGroup *vg)
{
    Mesh *m = 0;
    if(vg)
    {
        m = createMesh();
        if(m)
        {
            m->addGroup(vg);
            m_meshes.insert(pair<string, Mesh *>(name, m));
        }
        delete vg;
    }
    return m;
}

uint32_t RenderState::loadTextureFromFile(string name, string path, bool mipmaps)
{
    uint32_t texID = Material::textureFromTIFFImage(path, mipmaps);
    m_textures.insert(pair<string, uint32_t>(name, texID));
    return texID;
}

uint32_t RenderState::loadTextureFromData(string name, const char *data, size_t size, bool mipmaps)
{
    uint32_t texID = Material::textureFromTIFFImage(data, size, mipmaps);
    m_textures.insert(pair<string, uint32_t>(name, texID));
    return texID;
}

uint32_t RenderState::texture(string name) const
{
    map<string, uint32_t>::const_iterator it = m_textures.find(name);
    return (it != m_textures.end()) ? it->second : 0;
}

bool RenderState::drawNormals() const
{
    return m_drawNormals;
}

void RenderState::toggleNormals()
{
    m_drawNormals = !m_drawNormals;
}

void RenderState::toggleWireframe()
{
    m_wireframe = !m_wireframe;
}

void RenderState::toggleProjection()
{
    m_projection = !m_projection;
}

void RenderState::reset()
{
    m_output = Mesh::RenderToScreen;
    m_drawNormals = false;
    m_projection = true;
    m_wireframe = false;
}

void RenderState::drawMesh(string name)
{
    map<string, Mesh *>::iterator it = m_meshes.find(name);
    if(it != m_meshes.end())
        drawMesh(it->second);
}

void RenderState::beginExportMesh(string path)
{
    if(m_exporting)
        return;
    m_exporting = true;
    m_exportPath = path;
    m_oldOutput = m_output;
    pushMatrix();
    loadIdentity();
    m_output = Mesh::RenderToMesh;
    m_meshOutput = createMesh();
}

void RenderState::endExportMesh()
{
    if(!m_exporting)
        return;
    popMatrix();
    m_output = m_oldOutput;
    m_meshOutput->saveObj(m_exportPath);
    delete m_meshOutput;
    m_meshOutput = 0;
    m_exportPath = string();
    m_exporting = false;
}

void RenderState::init()
{
}

////////////////////////////////////////////////////////////////////////////////

StateObject::StateObject(RenderState *s)
{
    m_state = s;
}

void StateObject::loadIdentity()
{
    m_state->loadIdentity();
}

void StateObject::pushMatrix()
{
    m_state->pushMatrix();
}

void StateObject::popMatrix()
{
    m_state->popMatrix();
}

void StateObject::translate(float dx, float dy, float dz)
{
    m_state->translate(dx, dy, dz);
}

void StateObject::rotate(float angle, float rx, float ry, float rz)
{
    m_state->rotate(angle, rx, ry, rz);
}

void StateObject::scale(float sx, float sy, float sz)
{
    m_state->scale(sx, sy, sz);
}

void StateObject::drawMesh(Mesh *m)
{
    m_state->drawMesh(m);
}

void StateObject::drawMesh(string name)
{
    m_state->drawMesh(name);
}

void StateObject::pushMaterial(const Material &m)
{
    m_state->pushMaterial(m);
}

void StateObject::popMaterial()
{
    m_state->popMaterial();
}
