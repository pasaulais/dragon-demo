#ifndef INITIALS_DRAGON_H
#define INITIALS_DRAGON_H

#include "Material.h"

class DragonScene;

class Dragon
{
public:
    Dragon(DragonScene *scene);

    void setDetailLevel(int level);

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
    float alpha;              //
    float beta;               // animation-dependent variables
    float gamma;              //
    Material default_material;
    Material tongue_material;
    Material wing_material;
    Material wing_membrane_material;

private:
    DragonScene *m_scene;
    uint joint_parts;
    uint chest_parts;
    uint tail_end_parts;
};

#endif
