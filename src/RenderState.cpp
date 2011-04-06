#include <GL/gl.h>
#include "RenderState.h"

RenderState::RenderState(QObject *parent) : QObject(parent)
{
    m_meshOutput = 0;
    m_exporting = false;
    m_oldOutput = m_output;
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
    Mesh *m = Mesh::loadStl(path.toUtf8().constData(), this);
    if(m)
        m_meshes.insert(name, m);
    return m;
}

Mesh * RenderState::loadMeshObj(QString name, QString path)
{
    Mesh *m = Mesh::loadObj(path.toUtf8().constData(), this);
    if(m)
        m_meshes.insert(name, m);
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

void RenderState::reset()
{
    m_output = Mesh::Output_VertexList;
    m_drawNormals = false;
}

void RenderState::drawMesh(Mesh *m)
{
    if(!m)
        return;
    m->draw(m_output, this, m_meshOutput);
    if(m_drawNormals)
        m->drawNormals();
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
    glPushMatrix();
    glLoadIdentity();
    m_output = Mesh::Output_Mesh;
    m_meshOutput = new Mesh();
}

void RenderState::endExportMesh()
{
    if(!m_exporting)
        return;
    glPopMatrix();
    m_output = m_oldOutput;
    m_meshOutput->saveObj(m_exportPath);
    delete m_meshOutput;
    m_meshOutput = 0;
    m_exportPath = QString::null;
    m_exporting = false;
}

QMatrix4x4 RenderState::currentGLMatrix() const
{
    float m[4][4];
    glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);
    return QMatrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                     m[0][1], m[1][1], m[2][1], m[3][1],
                     m[0][2], m[1][2], m[2][2], m[3][2],
                     m[0][3], m[1][3], m[2][3], m[3][3]);
}

QMatrix4x4 RenderState::currentGLMatrixForNormals() const
{
    float m[4][4];
    glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);
    return QMatrix4x4(m[0][0], m[1][0], m[2][0], 0.0,
                     m[0][1], m[1][1], m[2][1], 0.0,
                     m[0][2], m[1][2], m[2][2], 0.0,
                     m[0][3], m[1][3], m[2][3], 1.0);
}
