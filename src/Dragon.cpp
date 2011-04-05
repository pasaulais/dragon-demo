#include <GL/gl.h>
#include <cmath>
#include "Dragon.h"
#include "DragonScene.h"
#include "Letters.h"

Dragon::Dragon(Kind kind, DragonScene *scene) : QObject(scene)
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
    m_tongueMaterial = Material(QVector4D(0.1, 0.0, 0.0, 1.0),
        QVector4D(0.6, 0.0, 0.0, 1.0), QVector4D(1.0, 1.0, 1.0, 1.0), 50.0);
    m_scalesMaterial = Material(QVector4D(0.2, 0.2, 0.2, 1.0),
        QVector4D(0.8, 0.8, 0.8, 1.0), QVector4D(1.0, 1.0, 1.0, 1.0), 20.0);
    m_wingMaterial = Material(QVector4D(0.2, 0.2, 0.2, 1.0),
        QVector4D(1.0, 1.0, 1.0, 1.0), QVector4D(1.0, 1.0, 1.0, 1.0), 20.0);
    m_membraneMaterial = Material(QVector4D(0.1, 0.0, 0.0, 1.0),
        QVector4D(0.6, 0.0, 0.0, 1.0), QVector4D(0.2, 0.2, 0.2, 1.0), 20.0);
    setDetailLevel(0);
    m_scene = scene;
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
    m_scalesMaterial.beginApply();
    glPushMatrix();
        glScalef(1.0/3.0, 1.0/3.0, 1.0/3.0);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(theta_neck, 0.0, 0.0, 1.0);
            glScalef(2.0, 2.0, 2.0);
            drawUpper();
        glPopMatrix();
        
        glPushMatrix();
            glScalef(3.0, 3.0, 3.0);
            drawBody();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-1.0, 0.0, 0.0);
            glRotatef(180.0, 0.0, 0.0, 1.0);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glScalef(2.0, 3.0, 3.0);
            drawTail();
        glPopMatrix();
    glPopMatrix();
    m_scalesMaterial.endApply();
}

void Dragon::drawUpper()
{
    glPushMatrix();
        glPushMatrix();
            glTranslatef(0.4, -0.04, 0.0);
            glRotatef(theta_head_y, 0.0, 1.0, 0.0);
            glRotatef(theta_head_z, 0.0, 0.0, 1.0);
            glScalef(0.6, 0.6, 0.6);
            drawHead();
        glPopMatrix();
        glPushMatrix();
            glScalef(0.5, 0.75, 0.75);
            drawJoint();
        glPopMatrix();
    glPopMatrix();
}

void Dragon::drawHead()
{
    glPushMatrix();
        // top of the head
        glPushMatrix();
            glScalef(1.0, 1.0, 3.0);
            Letters::drawP(m_scene);
        glPopMatrix();
        // left nostril
        glPushMatrix();
            glTranslatef(1.0, 0.49/7.0, -0.10);
            glScalef(0.33, 0.4, 1.2);
            glRotatef(180.0, 0.0, 1.0, 0.0);
            Letters::drawP(m_scene);
        glPopMatrix();
        // right nostril
        glPushMatrix();
            glTranslatef(1.0, 0.49/7.0, 0.10);
            glScalef(0.33, 0.4, 1.2);
            glRotatef(180.0, 0.0, 1.0, 0.0);
            Letters::drawP(m_scene);
        glPopMatrix();
        // tongue
        glPushMatrix();
            m_tongueMaterial.beginApply();
            glTranslatef(0.1, 0.0, 0.0);
            glRotatef(-theta_jaw, 0.0, 0.0, 1.0);
            glScalef(0.9, 0.9, 0.9);
            drawTongue();
            m_tongueMaterial.endApply();
        glPopMatrix();
        // jaw
        glPushMatrix();
            glRotatef(-theta_jaw, 0.0, 0.0, 1.0);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glScalef(1.0, 0.75, 0.5);
            Letters::drawA(m_scene);
        glPopMatrix();
    glPopMatrix();
}

void Dragon::drawTongue()
{
    glPushMatrix();
        glTranslatef(0.47, 0.0, 0.0);
        glScalef(1.1, 0.275, 1.1);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        Letters::drawS(m_scene);
    glPopMatrix();
}

void Dragon::drawJoint()
{
    float step = 360.0 / m_jointParts;
    for(float theta = 0.0; theta < 360.0; theta += step)
    {
        glPushMatrix();
            glTranslatef(0.40, 0.0, 0.0);
            glRotatef(theta, 1.0, 0.0, 0.0);
            glScalef(1.0, 0.5, 1.0);
            Letters::drawS(m_scene);
        glPopMatrix();
    }
}

void Dragon::drawBody()
{
    glPushMatrix();
        glScalef(1.0/3.0, 1.0/3.0, 1.0/3.0);
        glPushMatrix();
            glScalef(1.0, 0.5, 0.5);
            drawChest();
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.0, -0.3, 0.0);
            glScalef(1.3, 1.3, 1.3);
            drawPaws();
        glPopMatrix();
        
        // left wing
        m_wingMaterial.beginApply();
        glPushMatrix();
            glRotatef(theta_wing, 1.0, 0.0, 0.0);
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glScalef(3.0, 3.0, 3.0);
            drawWing();
        glPopMatrix();
        
        // right wing
        glPushMatrix();
            glRotatef(180.0, 0.0, 1.0, 0.0);
            glRotatef(theta_wing, 1.0, 0.0, 0.0);
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glScalef(3.0, 3.0, 3.0);
            drawWing();
        glPopMatrix();
        m_wingMaterial.endApply();
    glPopMatrix();
}

void Dragon::drawChest()
{
    float step = 360.0 / m_chestParts;
    for(float theta = 0.0; theta < 359.0; theta += step)
    {
        glPushMatrix();
            glTranslatef(-1.0, 0.0, 0.0);
            glRotatef(theta, 1.0, 0.0, 0.0);
            glScalef(2.0, 2.0, 2.0);
            Letters::drawP(m_scene);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(theta, 1.0, 0.0, 0.0);
            glRotatef(180.0, 0.0, 1.0, 0.0);
            glScalef(2.0, 2.0, 2.0);
            Letters::drawP(m_scene);
        glPopMatrix();
    }
}

void Dragon::drawWing()
{
    glPushMatrix();
        // scale both parts of the wing equally
        glScalef(0.5, 0.5, 0.5);
        drawWingPart();
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(-theta_wing_joint, 0.0, 0.0, 1.0);
            drawWingOuter();
        glPopMatrix();
    glPopMatrix();
}

void Dragon::drawWingPart()
{
    glPushMatrix();
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glScalef(1.0, 2.6, 0.20);
        Letters::drawA(m_scene);
    glPopMatrix();
    m_membraneMaterial.beginApply();
    glPushMatrix();
        glTranslatef(0.25, 0.0, 0.0);
        glScalef(0.26, 0.2, 0.2);
        drawWingMembrane();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.70, 0.0, 0.3);
        glScalef(0.3, 0.2, 0.27);
        drawWingMembrane();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.70, 0.0, -0.3);
        glScalef(0.3, 0.2, 0.27);
        drawWingMembrane();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(1.00, 0.0, 0.0);
        glRotatef(180.0, 0.0, 1.0, 0.0);
        glScalef(0.3, 0.2, 0.27);
        drawWingMembrane();
    glPopMatrix();
    m_membraneMaterial.endApply();
}

void Dragon::drawWingMembrane()
{
    glPushMatrix();
        glScalef(5.0, 5.0, 5.0);
        glPushMatrix();
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glScalef(0.2, 0.6, 0.05);
            Letters::drawA(m_scene);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.04, 0.0, 0.0);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glScalef(0.16, 0.36, 0.05);
            Letters::drawA(m_scene);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.2, 0.0, 0.12);
            glRotatef(270.0, 1.0, 0.0, 0.0);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glScalef(0.28, 0.4, 0.071);
            Letters::drawP(m_scene);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.2, 0.0, -0.12);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glScalef(0.28, 0.4, 0.071);
            Letters::drawP(m_scene);
        glPopMatrix();
    glPopMatrix();
}

void Dragon::drawWingOuter()
{
    glPushMatrix();
        glTranslatef(1.0, 0.0, 0.0);
        glRotatef(180.0, 0.0, 0.0, 1.0);
        drawWingPart();
    glPopMatrix();
}

void Dragon::drawPaws()
{
    glPushMatrix();
        glScalef(0.76, 0.76, 0.76);
        // front left paw
        glPushMatrix();
            glTranslatef(0.5, 0.0, -0.15);
            glRotatef(-theta_front_legs, 0.0, 0.0, 1.0);
            glRotatef(10.0, 0.0, 1.0, 0.0);
            glScalef(0.8, 0.8, 0.8);
            drawPaw();
        glPopMatrix();
        
        // front right paw
        glPushMatrix();
            glTranslatef(0.5, 0.0, 0.15);
            glRotatef(-theta_front_legs, 0.0, 0.0, 1.0);
            glRotatef(-10.0, 0.0, 1.0, 0.0);
            glScalef(0.8, 0.8, 0.8);
            drawPaw();
        glPopMatrix();
        
        // hind left paw
        glPushMatrix();
            glTranslatef(-0.5, 0.0, -0.15);
            glRotatef(-theta_back_legs, 0.0, 0.0, 1.0);
            glRotatef(10.0, 0.0, 1.0, 0.0);
            glScalef(1.2, 1.2, 1.2);
            drawPaw();
        glPopMatrix();
        
        // hind right paw
        glPushMatrix();
            glTranslatef(-0.5, 0.0, 0.15);
            glRotatef(-theta_back_legs, 0.0, 0.0, 1.0);
            glRotatef(-10.0, 0.0, 1.0, 0.0);
            glScalef(1.2, 1.2, 1.2);
            drawPaw();
        glPopMatrix();
    glPopMatrix();
}

void Dragon::drawPaw()
{
    glPushMatrix();
        glTranslatef(0.5, 0.0, 0.0);
        glRotatef(theta_paw, 0.0, 0.0, 1.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glScalef(0.5, 0.5, 0.5);
        Letters::drawA(m_scene);
    glPopMatrix();
    glPushMatrix();
        glScalef(0.6, 0.5, 0.5);
        drawJoint();
    glPopMatrix();
}

void Dragon::drawTail()
{
    GLuint n = 10;
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
    glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        // keep the transformation matrix for each joint,
        // so the transformations are applied on top of the previous ones
        glPushMatrix();
            glTranslatef(-0.40, 0.0, 0.0);
            glScalef(0.52, 1.0, 1.0);
            for(GLuint i = 0; i < n; i++)
            {
                float f = sizes[i];
                glTranslatef(0.80, 0.0, 0.0);
                glRotatef(angles[i] * mod[i], 0.0, 0.0, 1.0);
                glScalef(f, f, f);
                drawJoint();
            }
            glTranslatef(2.4, 0.0, 0.0);
            glRotatef(180.0, 0.0, 1.0, 0.0);
            glScalef(2.4, 1.8, 1.8);
            drawTailEnd();
        glPopMatrix();
    glPopMatrix();
}

void Dragon::drawTailEnd()
{
    float step = 360.0 / (m_tailEndParts * 2.0);
    for(float theta = 0.0; theta < 359.0; theta += step)
    {
        glPushMatrix();
            glRotatef(theta, 1.0, 0.0, 0.0);
            Letters::drawA(m_scene);
        glPopMatrix();
    }
}

void Dragon::animate(float t)
{
    theta_jaw = 10.0 * DragonScene::spaced_cos(t, 5.0, 2.0) + 10.0;
    theta_head_y = 45.0 * DragonScene::spaced_cos(t, 5.0, 2.0);
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
        theta_head_z = 60.0 * DragonScene::spaced_cos(t, 1.0, 2.0) - 30.0;
        theta_jaw = 10.0 * DragonScene::spaced_cos(t, 1.0, 2.0) + 10.0;
        break;
    }
}
