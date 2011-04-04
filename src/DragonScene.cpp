#include <GL/gl.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include "DragonScene.h"
#include "Mesh.h"
#include "Letters.h"

using namespace std;

static Material metal_material(QVector4D(0.2, 0.2, 0.2, 1.0),
    QVector4D(0.4, 0.4, 0.4, 1.0), QVector4D(0.2, 0.2, 0.2, 1.0), 20.0);

static Material red_material(QVector4D(0.2, 0.0, 0.0, 1.0),
    QVector4D(1.0, 0.0, 0.0, 1.0), QVector4D(0.0, 0.0, 0.0, 1.0), 20.0);

static Material green_material(QVector4D(0.0, 0.2, 0.0, 1.0),
    QVector4D(0.0, 1.0, 0.0, 1.0), QVector4D(0.0, 0.2, 0.0, 1.0), 20.0);

static Material blue_material(QVector4D(0.0, 0.0, 0.2, 1.0),
    QVector4D(0.0, 0.0, 0.1, 1.0), QVector4D(0.0, 0.0, 0.2, 1.0), 20.0);

static Material yellow_material(QVector4D(0.2, 0.2, 0.2, 1.0),
    QVector4D(1.0, 4.0/6.0, 0.0, 1.0), QVector4D(0.2, 0.2, 0.2, 1.0), 20.0);

static Material glass_material(QVector4D(0.2, 0.2, 0.2, 0.2),
    QVector4D(1.0, 1.0, 1.0, 0.2), QVector4D(0.2, 0.2, 0.2, 0.2), 20.0);

static Material debug_material(QVector4D(0.2, 0.2, 0.2, 1.0),
    QVector4D(1.0, 4.0/6.0, 0.0, 1.0), QVector4D(0.2, 0.2, 0.2, 1.0), 20.0);

DragonScene::DragonScene(QObject *parent) : Scene(parent)
{
    m_loadedMeshes = 0;
    m_missingMeshes = 0;
}

DragonScene::~DragonScene()
{
}

void DragonScene::reset()
{
    m_selected = SCENE;
    m_drawNormals = false;
    emit invalidated();
}

bool DragonScene::load()
{
    /*if(meshLoaded())
        return true;
    reset();
    return meshLoaded();*/
    return Letters::initMeshes();
}

Mesh * DragonScene::loadMesh(QString path)
{
    Mesh *m = Mesh::load_stl(path.toUtf8().constData(), this);
    if(m)
        m_loadedMeshes++;
    else
        m_missingMeshes++;
    return m;
}

bool DragonScene::meshLoaded() const
{
    return Letters::meshLoaded();
}

void DragonScene::draw()
{
    if(!meshLoaded())
        return;
    int item = m_selected;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    if(item == SCENE)
    {
        debug_material.beginApply();
        Letters::drawCube(this);
        debug_material.endApply();
    }
    else
    {
        debug_material.beginApply();
        switch(item)
        {
        case LETTER_P:
            Letters::drawP(this);
            break;
        case LETTER_A:
            Letters::drawA(this);
            break;
        case LETTER_S:
            Letters::drawS(this);
            break;
        }

        debug_material.endApply();
    }
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
}

void DragonScene::drawMesh(Mesh *m)
{
    if(!m)
        return;
    m->draw();
    if(m_drawNormals)
        m->draw_normals();
}

void DragonScene::animate()
{
    /*clock_t ticks = clock();
    GLfloat t = (GLfloat)ticks / (GLfloat)CLOCKS_PER_SEC;
    emit invalidated();*/
}
