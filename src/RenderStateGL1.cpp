#include <GL/gl.h>
#include <GL/glu.h>
#include "RenderStateGL1.h"

RenderStateGL1::RenderStateGL1(QObject *parent) : RenderState(parent)
{
    m_ambient0 = QVector4D(1.0, 1.0, 1.0, 1.0);
    m_diffuse0 = QVector4D(1.0, 1.0, 1.0, 1.0);
    m_specular0 = QVector4D(1.0, 1.0, 1.0, 1.0);
    m_light0_pos = QVector4D(0.0, 1.0, 1.0, 0.0);
}

void RenderStateGL1::drawMesh(Mesh *m)
{
    if(!m)
        return;
    m->draw(m_output, this, m_meshOutput);
    if(m_drawNormals)
        m->drawNormals(this);
}

void RenderStateGL1::setMatrixMode(RenderStateGL1::MatrixMode newMode)
{
    switch(newMode)
    {
    case ModelView:
        glMatrixMode(GL_MODELVIEW);
        break;
    case Projection:
        glMatrixMode(GL_PROJECTION);
        break;
    case Texture:
        glMatrixMode(GL_TEXTURE);
        break;
    }
}

void RenderStateGL1::loadIdentity()
{
    glLoadIdentity();
}

void RenderStateGL1::pushMatrix()
{
    glPushMatrix();
}

void RenderStateGL1::popMatrix()
{
    glPopMatrix();
}

void RenderStateGL1::translate(float dx, float dy, float dz)
{
    glTranslatef(dx, dy, dz);
}

void RenderStateGL1::rotate(float angle, float rx, float ry, float rz)
{
    glRotatef(angle, rx, ry, rz);
}

void RenderStateGL1::scale(float sx, float sy, float sz)
{
    glScalef(sx, sy, sz);
}

QMatrix4x4 RenderStateGL1::currentMatrix() const
{
    float m[4][4];
    glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);
    return QMatrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                     m[0][1], m[1][1], m[2][1], m[3][1],
                     m[0][2], m[1][2], m[2][2], m[3][2],
                     m[0][3], m[1][3], m[2][3], m[3][3]);
}

QMatrix4x4 RenderStateGL1::currentMatrixForNormals() const
{
    float m[4][4];
    glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);
    return QMatrix4x4(m[0][0], m[1][0], m[2][0], 0.0,
                     m[0][1], m[1][1], m[2][1], 0.0,
                     m[0][2], m[1][2], m[2][2], 0.0,
                     m[0][3], m[1][3], m[2][3], 1.0);
}

void RenderStateGL1::pushMaterial(const Material &m)
{
    m_materialStack.append(m);
    beginApplyMaterial(m);
}

void RenderStateGL1::popMaterial()
{
    m_materialStack.removeLast();
    endApplyMaterial();
    if(m_materialStack.count() > 0)
        beginApplyMaterial(m_materialStack.last());
}

void RenderStateGL1::beginApplyMaterial(const Material &m)
{
    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat *)&m.ambient());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat *)&m.diffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat *)&m.specular());
    glMaterialf(GL_FRONT, GL_SHININESS, m.shine());
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m.texture());
}

void RenderStateGL1::endApplyMaterial()
{
    glPopAttrib();
}

void RenderStateGL1::beginFrame(int w, int h)
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glEnable(GL_DEPTH_TEST);
    // we do non-uniform scaling and not all normals are one-unit-length
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&m_light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat *)&m_ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat *)&m_diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, (GLfloat *)&m_specular0);
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
    setupViewport(w, h);
    setMatrixMode(RenderStateGL1::ModelView);
    pushMatrix();
    loadIdentity();
    glClearColor(m_bgColor.redF(), m_bgColor.greenF(), m_bgColor.blueF(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderStateGL1::endFrame()
{
    glFlush();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setMatrixMode(RenderStateGL1::ModelView);
    popMatrix();
    glPopAttrib();
}

void RenderStateGL1::setupViewport(int w, int h)
{
    glViewport(0, 0, w, h);
    setMatrixMode(RenderStateGL1::Projection);
    loadIdentity();
    if(m_projection)
    {
        gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    }
    else
    {
        if (w <= h)
            glOrtho(-1.0, 1.0, -1.0 * (GLfloat) h / (GLfloat) w,
                1.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
        else
            glOrtho(-1.0 * (GLfloat) w / (GLfloat) h,
                1.0 * (GLfloat) w / (GLfloat) h, -1.0, 1.0, -10.0, 10.0);
    }
    setMatrixMode(RenderStateGL1::ModelView);
}
