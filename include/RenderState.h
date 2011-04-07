#ifndef INITIALS_RENDER_STATE_H
#define INITIALS_RENDER_STATE_H

#include <QObject>
#include <QColor>
#include <QMap>
#include "Mesh.h"
#include "Material.h"
#include "Vertex.h"

class RenderState : public QObject
{
public:
    RenderState(QObject *parent = 0);

    virtual bool drawNormals() const;
    virtual void toggleNormals();
    virtual void toggleWireframe();
    virtual void toggleProjection();

    virtual void reset();

    // mesh operations
    virtual void drawMesh(Mesh *m) = 0;
    virtual void drawMesh(QString name);

    virtual void beginExportMesh(QString path);
    virtual void endExportMesh();

    virtual QMap<QString, Mesh *> & meshes();
    virtual const QMap<QString, Mesh *> & meshes() const;

    virtual Mesh * loadMeshObj(QString name, QString path);
    virtual Mesh * loadMeshStl(QString name, QString path);

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
    QColor m_bgColor;
    Mesh *m_meshOutput;
    QMap<QString, Mesh *> m_meshes;

    // exporting
    bool m_exporting;
    QString m_exportPath;
    Mesh::OutputMode m_oldOutput;
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
