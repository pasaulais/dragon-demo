#ifndef INITIALS_DRAGON_H
#define INITIALS_DRAGON_H

#include <QObject>
#include "Material.h"
#include "RenderState.h"

class Scene;

class Dragon : public StateObject
{
public:
    enum Kind
    {
        Floating,
        Flying,
        Jumping
    };

    Dragon(Kind kind, RenderState *state,  QObject *parent = 0);

    static void loadMeshes(RenderState *scene);

    void setDetailLevel(int level);

    Material & tongueMaterial();
    Material & scalesMaterial();
    Material & wingMaterial();
    Material & membraneMaterial();

    float frontLegsAngle() const;
    float alpha() const;
    float beta() const;
    void setAlpha(float v);
    void setBeta(float v);

    void draw();

    void drawUpper();
    void drawHead();
    void drawTongue();
    void drawJoint();

    void drawBody();
    void drawChest();

    void drawWing();
    void drawWingPart();
    void drawWingOuter();
    void drawWingMembrane();

    void drawPaws();
    void drawPaw();

    void drawTail();
    void drawTailEnd();

    void animate(float t);

private:
    Kind m_kind;
    uint m_jointParts;
    uint m_chestParts;
    uint m_tailEndParts;
    Material m_tongueMaterial;
    Material m_scalesMaterial;
    Material m_wingMaterial;
    Material m_membraneMaterial;
    float theta_jaw;
    float theta_head_z;
    float theta_head_y;
    float theta_neck;
    float theta_wing;
    float theta_wing_joint;
    float theta_front_legs;
    float theta_back_legs;
    float theta_paw;
    float theta_tail;
    float m_alpha;              //
    float m_beta;               // animation-dependent variables
};

#endif
