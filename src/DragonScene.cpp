#include <GL/gl.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include "DragonScene.h"
#include "Dragon.h"
#include "Mesh.h"
#include "Material.h"
#include "Primitive.h"

using namespace std;

static Material debug_material(QVector4D(0.2, 0.2, 0.2, 1.0),
    QVector4D(1.0, 4.0/6.0, 0.0, 1.0), QVector4D(0.2, 0.2, 0.2, 1.0), 20.0);

static Material floor_material(QVector4D(0.5, 0.5, 0.5, 1.0),
    QVector4D(1.0, 1.0, 1.0, 1.0), QVector4D(1.0, 1.0, 1.0, 1.0), 00.0);

DragonScene::DragonScene(QObject *parent) : Scene(parent)
{
    m_meshOutput = 0;
    m_camera = Camera_Static;
    m_output = Mesh::Output_VertexList;

    m_debugDragon = new Dragon(Dragon::Floating, this);
    m_debugDragon->scalesMaterial() = debug_material;
    m_debugDragon->wingMaterial() = debug_material;
    m_dragons.append(new Dragon(Dragon::Floating, this));
    m_dragons.append(new Dragon(Dragon::Flying, this));
    m_dragons.append(new Dragon(Dragon::Jumping, this));
    animate();
}

void DragonScene::loadTextures()
{
    if(m_dragons.count() < 3)
        return;
    m_dragons.value(0)->scalesMaterial().loadTextureTIFF("scale_green.tiff");
    m_dragons.value(0)->wingMaterial().loadTextureTIFF("wing_green.tiff");
    m_dragons.value(1)->scalesMaterial().loadTextureTIFF("scale_black.tiff");
    m_dragons.value(1)->wingMaterial().loadTextureTIFF("wing_black.tiff");
    m_dragons.value(2)->scalesMaterial().loadTextureTIFF("scale_bronze.tiff");
    m_dragons.value(2)->wingMaterial().loadTextureTIFF("wing_bronze.tiff");
    floor_material.loadTextureTIFF("lava_green.tiff", true);
}

void DragonScene::freeTextures()
{
    foreach(Dragon *d, m_dragons)
    {
        d->scalesMaterial().freeTexture();
        d->wingMaterial().freeTexture();
    }
    floor_material.freeTexture();
}

void DragonScene::reset()
{
    m_selected = SCENE;
    m_drawNormals = false;
    m_theta = QVector3D(0, 0, 0);
    m_detailLevel = 4;
    m_camera = Camera_Static;
    emit invalidated();
}

QVector3D DragonScene::orientation() const
{
    if(m_selected == SCENE)
        return m_theta;
    else
        return QVector3D();
}

bool DragonScene::load()
{
    m_meshes.insert("floor", Primitive::createCube(this));
    loadMeshObj("letter_p", "meshes/LETTER_P.obj");
    loadMeshObj("letter_a", "meshes/LETTER_A.obj");
    loadMeshObj("letter_s", "meshes/LETTER_S.obj");
    Dragon::loadMeshes(this);
    return m_meshes.count() > 0;
}

Mesh * DragonScene::loadMeshStl(QString name, QString path)
{
    Mesh *m = Mesh::loadStl(path.toUtf8().constData(), this);
    if(m)
        m_meshes.insert(name, m);
    return m;
}

Mesh * DragonScene::loadMeshObj(QString name, QString path)
{
    Mesh *m = Mesh::loadObj(path.toUtf8().constData(), this);
    if(m)
        m_meshes.insert(name, m);
    return m;
}

QMap<QString, Mesh *> & DragonScene::meshes()
{
    return m_meshes;
}

const QMap<QString, Mesh *> & DragonScene::meshes() const
{
    return m_meshes;
}

void DragonScene::draw()
{
    Item i = (Item)m_selected;
    drawItem(i);
    if(m_selected != SCENE)
    {
        //exportItem(i, QString("meshes/%1.obj").arg(itemText(i)));
    }
}

void DragonScene::drawItem(DragonScene::Item item)
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
            drawMesh("letter_p");
            break;
        case LETTER_A:
            drawMesh("letter_a");
            break;
        case LETTER_S:
            drawMesh("letter_s");
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

void DragonScene::exportItem(Item item, QString path)
{
    Mesh::OutputMode oldOutput = m_output;
    glPushMatrix();
    glLoadIdentity();
    m_output = Mesh::Output_Mesh;
    m_meshOutput = new Mesh();
    drawItem(item);
    glPopMatrix();
    m_output = oldOutput;
    m_meshOutput->saveObj(path);
    delete m_meshOutput;
    m_meshOutput = 0;
}

QString DragonScene::itemText(DragonScene::Item item)
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

void DragonScene::drawScene()
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

void DragonScene::drawFloor()
{
    Mesh *floor = m_meshes.value("floor");
    floor_material.beginApply();
    glPushMatrix();
        glScalef(1.0, 0.001, 1.0);
        drawMesh(floor);
    glPopMatrix();
    floor_material.endApply();
}

void DragonScene::drawDragonHoldingA(Dragon *d)
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
            drawMesh("letter_a");
            d->tongueMaterial().endApply();
        glPopMatrix();
    glPopMatrix();
}

void DragonScene::drawDragonHoldingP(Dragon *d)
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
            drawMesh("letter_p");
            d->tongueMaterial().endApply();
        glPopMatrix();
    glPopMatrix();
}

void DragonScene::drawDragonHoldingS(Dragon *d)
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
            drawMesh("letter_s");
            d->tongueMaterial().endApply();
        glPopMatrix();
    glPopMatrix();
}

void DragonScene::select_next()
{
    if(m_selected < LAST)
    {
        m_selected++;
        emit invalidated();
    }
}

void DragonScene::select_previous()
{
    if(m_selected > SCENE)
    {
        m_selected--;
        emit invalidated();
    }
}

void DragonScene::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();
    if((key == Qt::Key_Plus) || (key == Qt::Key_Right))
        select_next();
    else if((key == Qt::Key_Minus)  || (key == Qt::Key_Left))
        select_previous();
    else if(key == Qt::Key_N)
        m_drawNormals = !m_drawNormals;
    else if(key == Qt::Key_F1)
        m_camera = Camera_Static;
    else if(key == Qt::Key_F2)
        m_camera = Camera_Flying;
    else if(key == Qt::Key_F3)
        m_camera = Camera_Jumping;
}

void DragonScene::drawMesh(Mesh *m)
{
    if(!m)
        return;
    m->draw(m_output, m_meshOutput);
    if(m_drawNormals)
        m->drawNormals();
}

void DragonScene::drawMesh(QString name)
{
    drawMesh(m_meshes.value(name));
}

void DragonScene::animate()
{
    clock_t ticks = clock();
    float t = (float)ticks / (float)CLOCKS_PER_SEC;

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
float DragonScene::sawtooth(float t)
{
    return t - (float)floor(t);
}

// Periodic function which returns 0 for w seconds then returns 1 for a seconds
float DragonScene::spaced_rect(float t, float w, float a)
{
    return (sawtooth(t / (w + a)) > w / (w + a)) ? 1.0 : 0.0;
}

// Periodic function which returns 0 for w seconds then is the sawtooth function for a seconds
float DragonScene::spaced_sawtooth(float x, float w, float a)
{
    return spaced_rect(x, w, a) * sawtooth((x - w) / (w + a)) * ((w + a) / a);
}

// Periodic function which returns 0 for w seconds then is the cosine function for a seconds
float DragonScene::spaced_cos(float x, float w, float a)
{
    return cos(2.0 * M_PI * spaced_sawtooth(x, w, a) + M_PI / 2.0);
}
