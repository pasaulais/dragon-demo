#ifndef INITIALS_RENDER_STATE_GL2_H
#define INITIALS_RENDER_STATE_GL2_H

#include <QList>
#include "RenderState.h"

class RenderStateGL2 : public RenderState
{
public:
    RenderStateGL2(QObject *parent = 0);

    virtual void drawMesh(Mesh *m);

    // matrix operations
    virtual void setMatrixMode(MatrixMode newMode);

    virtual void loadIdentity();
    virtual void multiplyMatrix(const matrix4 &m);
    virtual void pushMatrix();
    virtual void popMatrix();

    virtual void translate(float dx, float dy, float dz);
    virtual void rotate(float angle, float rx, float ry, float rz);
    virtual void scale(float sx, float sy, float sz);

    virtual matrix4 currentMatrix() const;

    // general state operations
    virtual void beginFrame(int width, int heigth);
    virtual void setupViewport(int width, int heigth);
    virtual void endFrame();

    // material operations
    virtual void pushMaterial(const Material &m);
    virtual void popMaterial();

private:
    void beginApplyMaterial(const Material &m);
    void endApplyMaterial();
    void compareMatrix(RenderState::MatrixMode mode) const;

    vec4 m_ambient0;
    vec4 m_diffuse0;
    vec4 m_specular0;
    vec4 m_light0_pos;
    QList<Material> m_materialStack;
    RenderState::MatrixMode m_matrixMode;
    matrix4 m_matrix[3];
    QList<matrix4> m_matrixStack[3];

    static bool useGL;
};

#endif