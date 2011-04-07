#include "RenderState.h"

RenderState::RenderState(QObject *parent) : QObject(parent)
{
    m_meshOutput = 0;
    m_exporting = false;
    m_oldOutput = m_output;
    m_bgColor = QColor::fromRgbF(0.6, 0.6, 1.0, 1.0);
    reset();
}

QMap<QString, Mesh *> & RenderState::meshes()
{
    return m_meshes;
}

const QMap<QString, Mesh *> & RenderState::meshes() const
{
    return m_meshes;
}

Mesh * RenderState::loadMeshStl(QString name, QString path)
{
    Mesh *m;
    VertexGroup *vg = Mesh::loadStl(path.toUtf8().constData());
    if(vg)
    {
        m = createMesh(vg);
        if(m)
            m_meshes.insert(name, m);
        delete vg;
    }
    return m;
}

Mesh * RenderState::loadMeshObj(QString name, QString path)
{
    Mesh *m;
    VertexGroup *vg = Mesh::loadObj(path.toUtf8().constData());
    if(vg)
    {
        m = createMesh(vg);
        if(m)
            m_meshes.insert(name, m);
        delete vg;
    }
    return m;
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

void RenderState::drawMesh(QString name)
{
    drawMesh(m_meshes.value(name));
}

void RenderState::beginExportMesh(QString path)
{
    if(m_exporting)
        return;
    m_exporting = true;
    m_exportPath = path;
    m_oldOutput = m_output;
    pushMatrix();
    loadIdentity();
    m_output = Mesh::RenderToMesh;
    m_meshOutput = createMesh(0);
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
    m_exportPath = QString::null;
    m_exporting = false;
}

void RenderState::init()
{
}

////////////////////////////////////////////////////////////////////////////////

StateObject::StateObject(RenderState *s, QObject *parent) : QObject(parent)
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

void StateObject::drawMesh(QString name)
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
