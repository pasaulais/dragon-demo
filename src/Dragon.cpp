#include <cmath>
#include "Dragon.h"
#include "RenderState.h"
#include "Scene.h"

Dragon::Dragon(Kind kind, RenderState *state) : StateObject(state)
{
    m_kind = kind;
    theta_jaw = 0.0;
    theta_head_z = 0.0;
    theta_head_y = 0.0;
    theta_neck = 0.0;
    theta_wing = 0.0;
    theta_wing_joint = 0.0;
    theta_front_legs = 0.0;
    theta_back_legs = 0.0;
    theta_paw = 0.0;
    theta_tail = 0.0;
    m_alpha = 0.0;
    m_beta = 0.0;
    m_tongueMaterial = Material(vec4(0.1, 0.0, 0.0, 1.0),
        vec4(0.6, 0.0, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 50.0);
    m_scalesMaterial = Material(vec4(0.2, 0.2, 0.2, 1.0),
        vec4(0.8, 0.8, 0.8, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 20.0);
    m_wingMaterial = Material(vec4(0.2, 0.2, 0.2, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 20.0);
    m_membraneMaterial = Material(vec4(0.1, 0.0, 0.0, 1.0),
        vec4(0.6, 0.0, 0.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 20.0);
    setDetailLevel(4);
}

float Dragon::frontLegsAngle() const
{
    return theta_front_legs;
}

float Dragon::alpha() const
{
    return m_alpha;
}

float Dragon::beta() const
{
    return m_beta;
}

void Dragon::setAlpha(float v)
{
    m_alpha = v;
}

void Dragon::setBeta(float v)
{
    m_beta = v;
}

void Dragon::setDetailLevel(int level)
{
    switch(level)
    {
        case 1:
            m_jointParts = 1;
            m_chestParts = 2;
            m_tailEndParts = 1;
            break;
        case 2:
            m_jointParts = 2;
            m_chestParts = 4;
            m_tailEndParts = 2;
            break;
        default:
        case 3:
            m_jointParts = 4;
            m_chestParts = 8;
            m_tailEndParts = 4;
            break;
        case 4:
            m_jointParts = 8;
            m_chestParts = 16;
            m_tailEndParts = 8;
            break;
    }
}

Material & Dragon::tongueMaterial()
{
    return m_tongueMaterial;
}

Material & Dragon::scalesMaterial()
{
    return m_scalesMaterial;
}

Material & Dragon::wingMaterial()
{
    return m_wingMaterial;
}

Material & Dragon::membraneMaterial()
{
    return m_membraneMaterial;
}

void Dragon::draw()
{
    pushMaterial(m_scalesMaterial);
    pushMatrix();
        scale(1.0/3.0, 1.0/3.0, 1.0/3.0);
        pushMatrix();
            translate(1.0, 0.0, 0.0);
            rotate(theta_neck, 0.0, 0.0, 1.0);
            scale(2.0, 2.0, 2.0);
            drawUpper();
        popMatrix();
        
        pushMatrix();
            scale(3.0, 3.0, 3.0);
            drawBody();
        popMatrix();

        pushMatrix();
            translate(-1.0, 0.0, 0.0);
            rotate(180.0, 0.0, 0.0, 1.0);
            rotate(90.0, 1.0, 0.0, 0.0);
            scale(2.0, 3.0, 3.0);
            drawTail();
        popMatrix();
    popMatrix();
    popMaterial();
}

void Dragon::drawUpper()
{
    pushMatrix();
        pushMatrix();
            translate(0.4, -0.04, 0.0);
            rotate(theta_head_y, 0.0, 1.0, 0.0);
            rotate(theta_head_z, 0.0, 0.0, 1.0);
            scale(0.6, 0.6, 0.6);
            drawHead();
        popMatrix();
        pushMatrix();
            scale(0.5, 0.75, 0.75);
            drawJoint();
        popMatrix();
    popMatrix();
}

void Dragon::drawHead()
{
    pushMatrix();
        drawMesh("dragon_head");
        // tongue
        pushMatrix();
            pushMaterial(m_tongueMaterial);
            translate(0.1, 0.0, 0.0);
            rotate(-theta_jaw, 0.0, 0.0, 1.0);
            scale(0.9, 0.9, 0.9);
            drawTongue();
            popMaterial();
        popMatrix();
        // jaw
        pushMatrix();
            rotate(-theta_jaw, 0.0, 0.0, 1.0);
            rotate(90.0, 1.0, 0.0, 0.0);
            scale(1.0, 0.75, 0.5);
            drawMesh("letter_a");
        popMatrix();
    popMatrix();
}

void Dragon::drawTongue()
{
    pushMatrix();
        translate(0.47, 0.0, 0.0);
        scale(1.1, 0.275, 1.1);
        rotate(180.0, 1.0, 0.0, 0.0);
        drawMesh("letter_s");
    popMatrix();
}

void Dragon::drawJoint()
{
    drawMesh("joint");
}

void Dragon::drawBody()
{
    pushMatrix();
        scale(1.0/3.0, 1.0/3.0, 1.0/3.0);
        pushMatrix();
            scale(1.0, 0.5, 0.5);
            drawChest();
        popMatrix();
        
        pushMatrix();
            translate(0.0, -0.3, 0.0);
            scale(1.3, 1.3, 1.3);
            drawPaws();
        popMatrix();
        
        // left wing
        pushMaterial(m_wingMaterial);
        pushMatrix();
            rotate(theta_wing, 1.0, 0.0, 0.0);
            rotate(90.0, 0.0, 1.0, 0.0);
            scale(3.0, 3.0, 3.0);
            drawWing();
        popMatrix();
        
        // right wing
        pushMatrix();
            rotate(180.0, 0.0, 1.0, 0.0);
            rotate(theta_wing, 1.0, 0.0, 0.0);
            rotate(90.0, 0.0, 1.0, 0.0);
            scale(3.0, 3.0, 3.0);
            drawWing();
        popMatrix();
        popMaterial();
    popMatrix();
}

void Dragon::drawChest()
{
    drawMesh("dragon_chest");
}

void Dragon::drawWing()
{
    pushMatrix();
        // scale both parts of the wing equally
        scale(0.5, 0.5, 0.5);
        drawWingPart();
        pushMatrix();
            translate(1.0, 0.0, 0.0);
            rotate(-theta_wing_joint, 0.0, 0.0, 1.0);
            drawWingOuter();
        popMatrix();
    popMatrix();
}

void Dragon::drawWingPart()
{
    pushMatrix();
        rotate(90.0, 1.0, 0.0, 0.0);
        scale(1.0, 2.6, 0.20);
        drawMesh("letter_a");
    popMatrix();
    pushMaterial(m_membraneMaterial);
    pushMatrix();
        translate(0.25, 0.0, 0.0);
        scale(0.26, 0.2, 0.2);
        drawWingMembrane();
    popMatrix();
    pushMatrix();
        translate(0.70, 0.0, 0.3);
        scale(0.3, 0.2, 0.27);
        drawWingMembrane();
    popMatrix();
    pushMatrix();
        translate(0.70, 0.0, -0.3);
        scale(0.3, 0.2, 0.27);
        drawWingMembrane();
    popMatrix();
    pushMatrix();
        translate(1.00, 0.0, 0.0);
        rotate(180.0, 0.0, 1.0, 0.0);
        scale(0.3, 0.2, 0.27);
        drawWingMembrane();
    popMatrix();
    popMaterial();
}

void Dragon::drawWingMembrane()
{
    drawMesh("wing_membrane");
}

void Dragon::drawWingOuter()
{
    pushMatrix();
        translate(1.0, 0.0, 0.0);
        rotate(180.0, 0.0, 0.0, 1.0);
        drawWingPart();
    popMatrix();
}

void Dragon::drawPaws()
{
    pushMatrix();
        scale(0.76, 0.76, 0.76);
        // front left paw
        pushMatrix();
            translate(0.5, 0.0, -0.15);
            rotate(-theta_front_legs, 0.0, 0.0, 1.0);
            rotate(10.0, 0.0, 1.0, 0.0);
            scale(0.8, 0.8, 0.8);
            drawPaw();
        popMatrix();
        
        // front right paw
        pushMatrix();
            translate(0.5, 0.0, 0.15);
            rotate(-theta_front_legs, 0.0, 0.0, 1.0);
            rotate(-10.0, 0.0, 1.0, 0.0);
            scale(0.8, 0.8, 0.8);
            drawPaw();
        popMatrix();
        
        // hind left paw
        pushMatrix();
            translate(-0.5, 0.0, -0.15);
            rotate(-theta_back_legs, 0.0, 0.0, 1.0);
            rotate(10.0, 0.0, 1.0, 0.0);
            scale(1.2, 1.2, 1.2);
            drawPaw();
        popMatrix();
        
        // hind right paw
        pushMatrix();
            translate(-0.5, 0.0, 0.15);
            rotate(-theta_back_legs, 0.0, 0.0, 1.0);
            rotate(-10.0, 0.0, 1.0, 0.0);
            scale(1.2, 1.2, 1.2);
            drawPaw();
        popMatrix();
    popMatrix();
}

void Dragon::drawPaw()
{
    pushMatrix();
        translate(0.5, 0.0, 0.0);
        rotate(theta_paw, 0.0, 0.0, 1.0);
        rotate(90.0, 1.0, 0.0, 0.0);
        scale(0.5, 0.5, 0.5);
        drawMesh("letter_a");
    popMatrix();
    pushMatrix();
        scale(0.6, 0.5, 0.5);
        drawJoint();
    popMatrix();
}

void Dragon::drawTail()
{
    uint32_t n = 10;
    float k = theta_tail / 20.0;
    static float sizes[10] =
    {
        // make the tail smaller and smaller as we get near the end
        1.0, 0.80, 0.75, 0.75, 0.77, 0.86, 0.9, 0.89, 0.88, 0.86
    };
    float angles[10] =
    {
        // rotate more and more each joint to make the tail curl
        -theta_tail, 0.0, 0.0, 45.0  * k, 45.0  * k, 60.0 * k, 45.0 * k, 60.0 * k, 120.0 * k, 60.0 * k
    };
    float mod[10] =
    {
        // slow down some joints by a factor inversely proportional to their size
        // 1.0, 0.80, 0.6, 0.45, 0.35, 0.30, 0.27, 0.24, 0.21, 0.18
        1.0, 1.0, 1.0, 0.6 * 0.45, 0.6 * 0.35, 0.6 * 0.30, 0.6 * 0.27, 0.6 * 0.24, 1.0, 1.0
    };
    pushMatrix();
        scale(0.5, 0.5, 0.5);
        // keep the transformation matrix for each joint,
        // so the transformations are applied on top of the previous ones
        pushMatrix();
            translate(-0.40, 0.0, 0.0);
            scale(0.52, 1.0, 1.0);
            for(uint32_t i = 0; i < n; i++)
            {
                float f = sizes[i];
                translate(0.80, 0.0, 0.0);
                rotate(angles[i] * mod[i], 0.0, 0.0, 1.0);
                scale(f, f, f);
                drawJoint();
            }
            translate(2.4, 0.0, 0.0);
            rotate(180.0, 0.0, 1.0, 0.0);
            scale(2.4, 1.8, 1.8);
            drawTailEnd();
        popMatrix();
    popMatrix();
}

void Dragon::drawTailEnd()
{
    drawMesh("dragon_tail_end");
}

void Dragon::animate(float t)
{
    theta_jaw = 10.0 * Scene::spaced_cos(t, 5.0, 2.0) + 10.0;
    theta_head_y = 45.0 * Scene::spaced_cos(t, 5.0, 2.0);
    theta_neck = 5.0 * cos(t * 3.0);
    theta_wing = 45.0 * cos(t * 3.5);
    theta_wing_joint = 60.0 - 30.0 * fabs(cos(t * 3.5) * cos(t));
    theta_front_legs = 10.0 * cos(t * 3.0) + 40.0 + 45.0;
    theta_back_legs = 10.0 * cos(t * 3.0) + 80.0 + 45.0;
    theta_tail = 15.0 * cos(pow(t * 0.3, 2.0)) * cos(6.0 * t * 0.3);
    switch(m_kind)
    {
    case Floating:
        theta_head_z = -45.0;
        theta_paw = 60.0;
        break;
    case Flying:
        theta_head_z = -30.0;
        theta_neck = 30.0;
        theta_paw = 60.0;
        break;
    case Jumping:
        theta_wing = 0.0;
        theta_wing_joint = 20.0;
        theta_neck = 30.0;
        theta_paw = 60.0;
        theta_neck = 30.0;
        theta_paw = 60.0;
        // this one is definitely having the time of its life
        theta_head_z = 60.0 * Scene::spaced_cos(t, 1.0, 2.0) - 30.0;
        theta_jaw = 10.0 * Scene::spaced_cos(t, 1.0, 2.0) + 10.0;
        break;
    }
}
