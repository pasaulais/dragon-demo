#include <GL/gl.h>
#include "Letters.h"
#include "Mesh.h"
#include "Primitive.h"
#include "DragonScene.h"

#define ANGLE_A             (73)
#define ANGLE_S             (180 * 1.15)
#define BASE_WIDTH_A        (0.15)

Mesh *Letters::arc_c = 0;
Mesh *Letters::arc_s = 0;
Mesh *Letters::disk = 0;
Mesh *Letters::cube = 0;
Mesh *Letters::sheared_a = 0;

bool Letters::meshLoaded()
{
    return arc_c != 0;
}

bool Letters::initMeshes()
{
    if(meshLoaded())
        return true;
    arc_c = Primitive::createCurve(180.0, 0.5, 16);
    arc_s = Primitive::createCurve(ANGLE_S, 0.5, 16);
    disk = Primitive::createCurve(180.0, 1.0, 16);
    cube = Primitive::createCube();
    sheared_a = Primitive::createShearedParalpd(BASE_WIDTH_A, 1.0, ANGLE_A);
    return true;
}

void Letters::drawP(DragonScene *s)
{
    glPushMatrix();
        glPushMatrix();
            glTranslatef(3.5/7.0, 0.5/7.0, 0.0/7.0);
            glScalef(3.5/7.0, 0.5/7.0, 0.5/7.0);
            s->drawMesh(cube);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(2.0/7.0, 1.0/7.0, 0.0/7.0);
            glScalef(2.0/7.0, 2.0/7.0, 1.0/7.0);
            s->drawMesh(arc_c);
        glPopMatrix();
    glPopMatrix();
}

void Letters::drawA(DragonScene *s)
{
    double bw = BASE_WIDTH_A;   // base width for the legs
    double y = 0.40;            // y position of the second bar, relative to the letter
    double m1w = Primitive::shearedParalpdWidth(bw, 1.0, ANGLE_A);
    glPushMatrix();
        glTranslatef(1.0, -m1w, -0.1);
        glRotatef(90, 0.0, 0.0, 1.0);
        // left leg
        glPushMatrix();
            glTranslatef((bw / 2.0), 0.0, 0.0);
            glScalef(1.0, 1.0, 0.2);
            s->drawMesh(sheared_a);
        glPopMatrix();
        // right leg
        glPushMatrix();
            glTranslatef(m1w * 2.0 - (bw / 2.0), 0.0, 0.2);
            glRotatef(180, 0.0, 1.0, 0.0);
            glScalef(1.0, 1.0, 0.2);
            s->drawMesh(sheared_a);
        glPopMatrix();
        // lower bar
        glPushMatrix();
            glTranslatef(m1w, y, 0.1);
            // make it slightly smaller to avoid texture problems
            glScalef(m1w / 2.0 * 0.99, 0.1 * 0.99, 0.1 * 0.99);
            s->drawMesh(cube);
        glPopMatrix();
    glPopMatrix();
}

void Letters::drawS(DragonScene *s)
{
    glPushMatrix();
        glRotatef(180, 1.0, 0.0, 0.0);
        glScalef(0.28, 0.28, 0.28);
        // top curve
        glPushMatrix();
            glTranslatef(0.75, 0.00, 0.00);
            glRotatef(180, 0.0, 0.0, 1.0);
            glScalef(1.0, 1.0, 0.5);
            s->drawMesh(arc_s);
        glPopMatrix();
        // bottom curve
        glPushMatrix();
            glTranslatef(-0.75, 0.00, 0.00);
            glScalef(1.0, 1.0, 0.5);
            s->drawMesh(arc_s);
        glPopMatrix();

        // round end of the top curve
        glPushMatrix();
            glTranslatef(0.75, 0.00, 0.00);
            glRotatef(ANGLE_S - 180, 0.0, 0.0, 1.0);
            glTranslatef(0.75, 0.00, 0.00);
            glScalef(0.25, 0.25, 0.5);
            s->drawMesh(disk);
        glPopMatrix();
        // round end of the bottom curve
        glPushMatrix();
            glTranslatef(-0.75, 0.00, 0.00);
            glRotatef(ANGLE_S, 0.0, 0.0, 1.0);
            glTranslatef(0.75, 0.00, 0.00);
            glScalef(0.25, 0.25, 0.5);
            s->drawMesh(disk);
        glPopMatrix();
    glPopMatrix();
}

void Letters::drawArcS(DragonScene *s)
{
    s->drawMesh(arc_s);
}

void Letters::drawCube(DragonScene *s)
{
    s->drawMesh(cube);
}
