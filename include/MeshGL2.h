#ifndef INITIALS_MESH_GL2_H
#define INITIALS_MESH_GL2_H

#include <QObject>
#include <QList>
#include "Mesh.h"
#include "Vertex.h"

class RenderState;
class RenderStateGL2;

class MeshGL2 : public Mesh
{
public:
    MeshGL2(const RenderStateGL2 *state, QObject *parent = 0);
    virtual ~MeshGL2();

    virtual int groupCount() const;
    virtual uint groupMode(int index) const;
    virtual uint groupSize(int index) const;
    virtual void addGroup(VertexGroup *vg);
    virtual bool copyGroupTo(int index, VertexGroup *vg) const;
    virtual void draw(OutputMode mode, RenderState *s, Mesh *output = 0);

private:
    void drawToScreen();
    void drawArray(VertexGroup *vg, int position, int normal, int texCoords);
    void drawVBO(VertexGroup *vg, int position, int normal, int texCoords);

    const RenderStateGL2 *m_state;
    QList<VertexGroup *> m_groups;
};

#endif
