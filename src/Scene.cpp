#include <GL/gl.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include "Scene.h"
#include "Dragon.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderState.h"

using namespace std;

static Material debug_material(QVector4D(0.2, 0.2, 0.2, 1.0),
    QVector4D(1.0, 4.0/6.0, 0.0, 1.0), QVector4D(0.2, 0.2, 0.2, 1.0), 20.0);

static Material floor_material(QVector4D(0.5, 0.5, 0.5, 1.0),
    QVector4D(1.0, 1.0, 1.0, 1.0), QVector4D(1.0, 1.0, 1.0, 1.0), 00.0);

Scene::Scene(RenderState *state, QObject *parent) : QObject(parent)
{
    m_state = state;
    m_camera = Camera_Static;
    m_exportQueued = false;

    m_debugDragon = new Dragon(Dragon::Floating, m_state, this);
    m_debugDragon->scalesMaterial() = debug_material;
    m_debugDragon->wingMaterial() = debug_material;
    m_dragons.append(new Dragon(Dragon::Floating, m_state, this));
    m_dragons.append(new Dragon(Dragon::Flying, m_state, this));
    m_dragons.append(new Dragon(Dragon::Jumping, m_state, this));
    animate();
}

void Scene::loadTextures()
{
    if(m_dragons.count() < 3)
        return;
    m_dragons.value(0)->scalesMaterial().loadTextureTIFF("scale_green.tiff");
    m_dragons.value(0)->wingMaterial().loadTextureTIFF("scale_green.tiff");
    m_dragons.value(1)->scalesMaterial().loadTextureTIFF("scale_black.tiff");
    m_dragons.value(1)->wingMaterial().loadTextureTIFF("scale_black.tiff");
    m_dragons.value(2)->scalesMaterial().loadTextureTIFF("scale_bronze.tiff");
    m_dragons.value(2)->wingMaterial().loadTextureTIFF("scale_bronze.tiff");
    floor_material.loadTextureTIFF("lava_green.tiff", true);
}

void Scene::freeTextures()
{
    foreach(Dragon *d, m_dragons)
    {
        d->scalesMaterial().freeTexture();
        d->wingMaterial().freeTexture();
    }
    floor_material.freeTexture();
}

void Scene::reset()
{
    m_selected = SCENE;
    m_theta = QVector3D(0, 0, 0);
    m_detailLevel = 4;
    m_camera = Camera_Static;
    emit invalidated();
}

QVector3D Scene::orientation() const
{
    if(m_selected == SCENE)
        return m_theta;
    else
        return QVector3D();
}

bool Scene::load()
{
    m_state->loadMeshObj("floor", "meshes/floor.obj");
    m_state->loadMeshObj("letter_p", "meshes/LETTER_P.obj");
    m_state->loadMeshObj("letter_a", "meshes/LETTER_A.obj");
    m_state->loadMeshObj("letter_s", "meshes/LETTER_S.obj");
    Dragon::loadMeshes(m_state);
    return m_state->meshes().count() > 0;
}

void Scene::draw()
{
    Item i = (Item)m_selected;
    drawItem(i);
    if(m_exportQueued)
    {
        exportItem(i, QString("meshes/%1.obj").arg(itemText(i)));
        m_exportQueued = false;
    }
}

void Scene::drawItem(Scene::Item item)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    if(item == SCENE)
    {
        drawScene();
    }
    else
    {
        debug_material.beginApply();
        switch(item)
        {
        case LETTER_P:
            m_state->drawMesh("letter_p");
            break;
        case LETTER_A:
            m_state->drawMesh("letter_a");
            break;
        case LETTER_S:
            m_state->drawMesh("letter_s");
            break;
        case DRAGON:
            m_debugDragon->draw();
            break;
        case DRAGON_UPPER:
            m_debugDragon->drawUpper();
            break;
        case DRAGON_HEAD:
            m_debugDragon->drawHead();
            break;
        case DRAGON_TONGUE:
            m_debugDragon->drawTongue();
            break;
        case DRAGON_JOINT:
            m_debugDragon->drawJoint();
            break;
        case DRAGON_BODY:
            m_debugDragon->drawBody();
            break;
        case DRAGON_CHEST:
            m_debugDragon->drawChest();
            break;
        case DRAGON_PAWS:
            m_debugDragon->drawPaws();
            break;
        case DRAGON_PAW:
            m_debugDragon->drawPaw();
            break;
        case DRAGON_WING:
            m_debugDragon->drawWing();
            break;
        case DRAGON_WING_OUTER:
            m_debugDragon->drawWingOuter();
            break;
        case DRAGON_WING_PART:
            m_debugDragon->drawWingPart();
            break;
        case DRAGON_WING_MEMBRANE:
            m_debugDragon->drawWingMembrane();
            break;
        case DRAGON_TAIL:
            m_debugDragon->drawTail();
            break;
        case DRAGON_TAIL_END:
            m_debugDragon->drawTailEnd();
            break;
        }
        debug_material.endApply();

    }
    glPopMatrix();
}

void Scene::exportItem(Item item, QString path)
{
    m_state->beginExportMesh(path);
    drawItem(item);
    m_state->endExportMesh();
}

QString Scene::itemText(Scene::Item item)
{
    switch(item)
    {
    case LETTER_P:
        return "LETTER_P";
    case LETTER_A:
        return "LETTER_A";
    case LETTER_S:
        return "LETTER_S";
    case DRAGON:
        return "DRAGON";
    case DRAGON_UPPER:
        return "DRAGON_UPPER";
    case DRAGON_HEAD:
        return "DRAGON_HEAD";
    case DRAGON_TONGUE:
        return "DRAGON_TONGUE";
    case DRAGON_JOINT:
        return "DRAGON_JOINT";
    case DRAGON_BODY:
        return "DRAGON_BODY";
    case DRAGON_CHEST:
        return "DRAGON_CHEST";
    case DRAGON_PAWS:
        return "DRAGON_PAWS";
    case DRAGON_PAW:
        return "DRAGON_PAW";
    case DRAGON_WING:
        return "DRAGON_WING";
    case DRAGON_WING_OUTER:
        return "DRAGON_WING_OUTER";
    case DRAGON_WING_PART:
        return "DRAGON_WING_PART";
    case DRAGON_WING_MEMBRANE:
        return "DRAGON_WING_MEMBRANE";
    case DRAGON_TAIL:
        return "DRAGON_TAIL";
    case DRAGON_TAIL_END:
        return "DRAGON_TAIL_END";
    case SCENE:
        return "SCENE";
    }
    return "UNKNOWN";
}

void Scene::drawScene()
{
    // tile the texture 16 times in both directions
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
        glScalef(16.0, 16.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glTranslatef(0.0, -2.0, 0.0);
            glScalef(100.0, 1.0, 100.0);
            drawFloor();
        glPopMatrix();
        glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    Dragon *da = m_dragons[0];
    glPushMatrix();
        glTranslatef(0.0, 2.0 + 0.6 * da->alpha(), 0.0);
        glScalef(3.0, 3.0, 3.0);
        da->setDetailLevel(m_detailLevel);
        drawDragonHoldingA(da);
    glPopMatrix();

    Dragon *dp = m_dragons[1];
    glPushMatrix();
        glTranslatef(-dp->beta(), dp->beta(), dp->beta());
        glRotatef(dp->alpha(), 0.0, 1.0, 0.0);
        glTranslatef(4.0, 0.0, 4.0);
        glRotatef(60.0, 0.0, 1.0, 0.0);
        glScalef(1.5, 1.5, 1.5);
        dp->setDetailLevel(m_detailLevel);
        drawDragonHoldingP(dp);
    glPopMatrix();

    Dragon *ds = m_dragons[2];
    glPushMatrix();
        glTranslatef(0.0, ds->beta(), 0.0);
        glRotatef(-ds->alpha(), 0.0, 1.0, 0.0);
        glTranslatef(3.0, 0.0, 3.0);
        glRotatef(-120.0, 0.0, 1.0, 0.0);
        glScalef(1.5, 1.5, 1.5);
        ds->setDetailLevel(m_detailLevel);
        drawDragonHoldingS(ds);
    glPopMatrix();
}

void Scene::drawFloor()
{
    floor_material.beginApply();
    glPushMatrix();
        glScalef(1.0, 0.001, 1.0);
        m_state->drawMesh("floor");
    glPopMatrix();
    floor_material.endApply();
}

void Scene::drawDragonHoldingA(Dragon *d)
{
    glPushMatrix();
        glPushMatrix();
            glRotatef(45.0, 0.0, 0.0, 1.0);
            d->draw();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0/3.0, 0.2/3.0, 0.0);
            glRotatef(15.0, 0.0, 1.0, 0.0);
            glRotatef(-d->frontLegsAngle(), 0.0, 0.0, 1.0);
            glScalef(2.0/3.0, 2.0/3.0, 1.0/3.0);
            d->tongueMaterial().beginApply();
            m_state->drawMesh("letter_a");
            d->tongueMaterial().endApply();
        glPopMatrix();
    glPopMatrix();
}

void Scene::drawDragonHoldingP(Dragon *d)
{
    glPushMatrix();
        d->draw();
        glPushMatrix();
            glTranslatef(0.08, -0.13, 0.0);
            glRotatef(-d->frontLegsAngle() + 90.0, 0.0, 0.0, 1.0);
            glTranslatef(0.2, -0.1, 0.0);
            glRotatef(-170, 0.0, 0.0, 1.0);
            glScalef(1.0, 1.0, 0.5);
            d->tongueMaterial().beginApply();
            m_state->drawMesh("letter_p");
            d->tongueMaterial().endApply();
        glPopMatrix();
    glPopMatrix();
}

void Scene::drawDragonHoldingS(Dragon *d)
{
    glPushMatrix();
        d->draw();
        glPushMatrix();
            glTranslatef(0.26, -0.25, 0.0);
            glRotatef(180.0 - d->frontLegsAngle(), 0.0, 0.0, 1.0);
            // need to change the center of the rotation
            glTranslatef(-0.4, 0.1, 0.0);
            glScalef(1.0, 1.0, 0.5);
            d->tongueMaterial().beginApply();
            m_state->drawMesh("letter_s");
            d->tongueMaterial().endApply();
        glPopMatrix();
    glPopMatrix();
}

void Scene::select_next()
{
    if(m_selected < LAST)
    {
        m_selected++;
        emit invalidated();
    }
}

void Scene::select_previous()
{
    if(m_selected > SCENE)
    {
        m_selected--;
        emit invalidated();
    }
}

void Scene::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();
    if((key == Qt::Key_Plus) || (key == Qt::Key_Right))
        select_next();
    else if((key == Qt::Key_Minus)  || (key == Qt::Key_Left))
        select_previous();
    else if(key == Qt::Key_N)
        m_state->toggleNormals();
    else if(key == Qt::Key_F1)
        m_camera = Camera_Static;
    else if(key == Qt::Key_F2)
        m_camera = Camera_Flying;
    else if(key == Qt::Key_F3)
        m_camera = Camera_Jumping;
    else if(key == Qt::Key_S)
        m_exportQueued = true;
}

void Scene::animate()
{
    clock_t ticks = clock();
    float t = ((float)ticks / (float)CLOCKS_PER_SEC) * 2.0;

    double angle = fmod(t * 45.0, 360.0);

    // hovering dragon
    m_dragons[0]->animate(t);
    m_dragons[0]->setAlpha(cos(t * 3.5 + M_PI));

    // drunk dragon trying to fly clockwise
    m_dragons[1]->animate(t);
    m_dragons[1]->setAlpha(angle);
    m_dragons[1]->setBeta(cos(t * 3.5) * cos(t) * cos(t));

    // dragon jumping anticlockwise
    m_dragons[2]->animate(t);
    m_dragons[2]->setAlpha(angle);
    m_dragons[2]->setBeta(1.20 * sqrt(fabs(cos(5.0 * t) - cos(6.0 * t) + cos(7.0 * t))));

    switch(m_camera)
    {
    default:
    case Camera_Static:
        m_theta.setY(0.0);			// static camera
        break;
    case Camera_Jumping:
        m_theta.setY(angle);		// following jumping dragon
        break;
    case Camera_Flying:
        m_theta.setY(-angle);       // following drunk dragon
        break;
    }

    emit invalidated();
}

// Periodic function linearly going from 0 to 1
float Scene::sawtooth(float t)
{
    return t - (float)floor(t);
}

// Periodic function which returns 0 for w seconds then returns 1 for a seconds
float Scene::spaced_rect(float t, float w, float a)
{
    return (sawtooth(t / (w + a)) > w / (w + a)) ? 1.0 : 0.0;
}

// Periodic function which returns 0 for w seconds then is the sawtooth function for a seconds
float Scene::spaced_sawtooth(float x, float w, float a)
{
    return spaced_rect(x, w, a) * sawtooth((x - w) / (w + a)) * ((w + a) / a);
}

// Periodic function which returns 0 for w seconds then is the cosine function for a seconds
float Scene::spaced_cos(float x, float w, float a)
{
    return cos(2.0 * M_PI * spaced_sawtooth(x, w, a) + M_PI / 2.0);
}
