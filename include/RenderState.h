#ifndef INITIALS_RENDER_STATE_H
#define INITIALS_RENDER_STATE_H

#include <QObject>
#include <QMatrix4x4>
#include <QMap>
#include "Mesh.h"

class RenderState : public QObject
{
public:
    RenderState(QObject *parent = 0);

    void drawMesh(Mesh *m);
    void drawMesh(QString name);

    bool drawNormals() const;
    void toggleNormals();

    void reset();

    void beginExportMesh(QString path);
    void endExportMesh();

    QMap<QString, Mesh *> & meshes();
    const QMap<QString, Mesh *> & meshes() const;

    Mesh * loadMeshObj(QString name, QString path);
    Mesh * loadMeshStl(QString name, QString path);

    QMatrix4x4 currentGLMatrix() const;
    QMatrix4x4 currentGLMatrixForNormals() const;

private:
    Mesh::OutputMode m_output;
    bool m_drawNormals;
    Mesh *m_meshOutput;
    QMap<QString, Mesh *> m_meshes;
    // exporting
    bool m_exporting;
    QString m_exportPath;
    Mesh::OutputMode m_oldOutput;
    // material stack?
};

#endif
