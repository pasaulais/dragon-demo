#ifndef INITIALS_DRAGON_H
#define INITIALS_DRAGON_H

#include "Mesh.h"

class DragonScene;

class Dragon
{
public:
    Dragon(DragonScene *scene);

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

    GLfloat theta_jaw;
    GLfloat theta_head_z;
    GLfloat theta_head_y;
    GLfloat theta_neck;
    GLfloat theta_wing;
    GLfloat theta_wing_joint;
    GLfloat theta_front_legs;
    GLfloat theta_back_legs;
    GLfloat theta_paw;
    GLfloat theta_tail;
    GLfloat alpha;              //
    GLfloat beta;               // animation-dependent variables
    GLfloat gamma;              //
    GLuint joint_parts;
    GLuint chest_parts;
    GLuint tail_end_parts;
    Material default_material;
    Material tongue_material;
    Material wing_material;
    Material wing_membrane_material;

private:
    DragonScene *m_scene;
};

#endif
