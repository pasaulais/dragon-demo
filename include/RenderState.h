#ifndef INITIALS_RENDER_STATE_H
#define INITIALS_RENDER_STATE_H

#include <QObject>
#include <QMatrix4x4>
#include <QColor>
#include <QMap>
#include <QList>
#include "Mesh.h"
#include "Material.h"

class RenderState : public QObject
{
public:
    RenderState(QObject *parent = 0);

    bool drawNormals() const;
    void toggleNormals();
    void toggleWireframe();
    void toggleProjection();

    void reset();

    // mesh operations
    void drawMesh(Mesh *m);
    void drawMesh(QString name);

    void beginExportMesh(QString path);
    void endExportMesh();

    QMap<QString, Mesh *> & meshes();
    const QMap<QString, Mesh *> & meshes() const;

    Mesh * loadMeshObj(QString name, QString path);
    Mesh * loadMeshStl(QString name, QString path);

    // matrix operations

    enum MatrixMode
    {
        ModelView,
        Projection,
        Texture
    };

    void setMatrixMode(MatrixMode newMode);

    void loadIdentity();
    void pushMatrix();
    void popMatrix();

    void translate(float dx, float dy, float dz);
    void rotate(float angle, float rx, float ry, float rz);
    void scale(float sx, float sy, float sz);

    QMatrix4x4 currentGLMatrix() const;
    QMatrix4x4 currentGLMatrixForNormals() const;

    // general state operations
    void beginFrame(int width, int heigth);
    void setupViewport(int width, int heigth);
    void endFrame();

    // material operations

    void pushMaterial(const Material &m);
    void popMaterial();

private:
    void beginApplyMaterial(const Material &m);
    void endApplyMaterial();

    Mesh::OutputMode m_output;
    bool m_drawNormals;
    bool m_projection;
    bool m_wireframe;
    QColor m_bgColor;
    QVector4D m_ambient0;
    QVector4D m_diffuse0;
    QVector4D m_specular0;
    QVector4D m_light0_pos;

    Mesh *m_meshOutput;
    QMap<QString, Mesh *> m_meshes;

    // exporting
    bool m_exporting;
    QString m_exportPath;
    Mesh::OutputMode m_oldOutput;
    QList<Material> m_materialStack;
};

class StateObject : public QObject
{
public:
    StateObject(RenderState *s, QObject *parent = 0);

    void loadIdentity();
    void pushMatrix();
    void popMatrix();

    void translate(float dx, float dy, float dz);
    void rotate(float angle, float rx, float ry, float rz);
    void scale(float sx, float sy, float sz);

    void drawMesh(Mesh *m);
    void drawMesh(QString name);

    void pushMaterial(const Material &m);
    void popMaterial();

protected:
    RenderState *m_state;
};

#endif
