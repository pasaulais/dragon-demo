#ifdef JNI_WRAPPER
#include <GLES/gl.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "RenderStateGL1.h"
#include "MeshGL1.h"

RenderStateGL1::RenderStateGL1() : RenderState()
{
    m_ambient0 = vec4(1.0, 1.0, 1.0, 1.0);
    m_diffuse0 = vec4(1.0, 1.0, 1.0, 1.0);
    m_specular0 = vec4(1.0, 1.0, 1.0, 1.0);
    m_light0_pos = vec4(0.0, 1.0, 1.0, 0.0);
}

Mesh * RenderStateGL1::createMesh() const
{
    return new MeshGL1();
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

void RenderStateGL1::multiplyMatrix(const matrix4 &m)
{
    glMultMatrixf((const GLfloat *)m.d);
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

matrix4 RenderStateGL1::currentMatrix() const
{
    matrix4 m;
    glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m.d);
    return m;
}

void RenderStateGL1::pushMaterial(const Material &m)
{
    m_materialStack.push_back(m);
    beginApplyMaterial(m);
}

void RenderStateGL1::popMaterial()
{
    Material m = m_materialStack.back();
    m_materialStack.pop_back();
    endApplyMaterial(m);
    if(m_materialStack.size() > 0)
        beginApplyMaterial(m_materialStack.back());
}

void RenderStateGL1::beginApplyMaterial(const Material &m)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat *)&m.ambient());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat *)&m.diffuse());
    glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat *)&m.specular());
    glMaterialf(GL_FRONT, GL_SHININESS, m.shine());
    if(m.texture() != 0)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m.texture());
    }
}

void RenderStateGL1::endApplyMaterial(const Material &m)
{
    if(m.texture() != 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

void RenderStateGL1::beginFrame(int w, int h)
{
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
#ifndef JNI_WRAPPER
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
#endif
    setupViewport(w, h);
    setMatrixMode(RenderStateGL1::ModelView);
    pushMatrix();
    loadIdentity();
    glClearColor(m_bgColor.x, m_bgColor.y, m_bgColor.z, m_bgColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderStateGL1::endFrame()
{
    glFlush();
#ifndef JNI_WRAPPER
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    setMatrixMode(RenderStateGL1::ModelView);
    popMatrix();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

void RenderStateGL1::setupViewport(int w, int h)
{
    glViewport(0, 0, w, h);
    setMatrixMode(Projection);
    loadIdentity();
    float r = (float)w / (float)h;
    if(m_projection)
        multiplyMatrix(matrix4::perspective(45.0f, r, 0.1f, 100.0f));
    else if (w <= h)
        multiplyMatrix(matrix4::ortho(-1.0, 1.0, -1.0 / r, 1.0 / r, -10.0, 10.0));
    else
        multiplyMatrix(matrix4::ortho(-1.0 * r, 1.0 * r, -1.0, 1.0, -10.0, 10.0));
    setMatrixMode(ModelView);
}
