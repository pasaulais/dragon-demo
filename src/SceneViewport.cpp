// Copyright (c) 2009-2015, Pierre-Andre Saulais <pasaulais@free.fr>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <cmath>
#include "Platform.h"
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include "SceneViewport.h"
#include "Vertex.h"
#include "Scene.h"
#include "Material.h"
#include "RenderState.h"

SceneViewport::SceneViewport(Scene *scene, RenderState *state, const QGLFormat &format, QWidget *parent) : QGLWidget(format, parent)
{
    setMinimumSize(640, 480);
    m_scene = scene;
    m_state = state;
    m_renderTimer = new QTimer(this);
    m_renderTimer->setInterval(0);
    m_frames = 0;
    m_lastFPS = 0;
    m_fpsTimer = new QTimer(this);
    m_fpsTimer->setInterval(1000 / 10);
    setAutoFillBackground(false);
    connect(m_fpsTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
    connect(m_renderTimer, SIGNAL(timeout()), this, SLOT(animateScene()));
}

SceneViewport::~SceneViewport()
{
    makeCurrent();
    m_state->freeTextures();
}

void SceneViewport::initializeGL()
{
    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        fprintf(stderr, "GLEW Error: %s", glewGetErrorString(err));
        return;
    }
    m_state->init();
    m_scene->init();
    resetCamera();
}

void SceneViewport::resizeGL(int w, int h)
{
    m_state->setupViewport(w, h);
}

void SceneViewport::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paintGL();
    m_frames++;
    if(m_fpsTimer->isActive())
        paintFPS(&painter, m_lastFPS);
}

void SceneViewport::paintGL()
{
    m_state->beginFrame(width(), height());
    m_scene->draw();
    m_state->endFrame();
}

void SceneViewport::resetCamera()
{
    m_transState.last = vec3();
    m_rotState.last = vec3();
    m_transState.active = false;
    m_rotState.active = false;
    m_animate = true;
    m_state->reset();
    m_scene->reset();
    updateAnimationState();
}

void SceneViewport::toggleAnimation()
{
    m_animate = !m_animate;
    if(m_animate)
        m_renderTimer->start();
    else
        m_renderTimer->stop();
}

void SceneViewport::startFPS()
{
    m_start = QDateTime::currentDateTime();
}

void SceneViewport::updateFPS()
{
    qint64 elapsedMillis = m_start.msecsTo(QDateTime::currentDateTime());
    m_lastFPS = m_frames / ((float)elapsedMillis / 1000.0);
    m_frames = 0;
    m_start = QDateTime::currentDateTime();
}

void SceneViewport::paintFPS(QPainter *p, float fps)
{
    QFont f;
    f.setPointSizeF(16.0);
    f.setWeight(QFont::Bold);
    p->setFont(f);
    QString text = QString("%1 FPS").arg(fps, 0, 'g', 4);
    p->setPen(QPen(Qt::white));
    p->drawText(QRectF(QPointF(10, 5), QSizeF(100, 100)), text);
}

void SceneViewport::updateAnimationState()
{
    if(m_animate)
    {
        startFPS();
        m_renderTimer->start();
        m_fpsTimer->start();
    }
    else
    {
        m_renderTimer->stop();
        m_fpsTimer->stop();
    }
}

void SceneViewport::animateScene()
{
    m_scene->animate();
    update();
}

void SceneViewport::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key == Qt::Key_R)
        resetCamera();
    if((key == Qt::Key_Plus) || (key == Qt::Key_Right))
        m_scene->selectNext();
    else if((key == Qt::Key_Minus)  || (key == Qt::Key_Left))
        m_scene->selectPrevious();
    else if(key == Qt::Key_Q)
        m_scene->theta().y += 5.0;
    else if(key == Qt::Key_D)
        m_scene->theta().y -= 5.0;
    else if(key == Qt::Key_2)
        m_scene->theta().x += 5.0;
    else if(key == Qt::Key_8)
        m_scene->theta().x -= 5.0;
    else if(key == Qt::Key_4)
        m_scene->theta().z += 5.0;
    else if(key == Qt::Key_6)
        m_scene->theta().z -= 5.0;
    else if(key == Qt::Key_7)
        m_scene->topView();
    else if(key == Qt::Key_3)
        m_scene->sideView();
    else if(key == Qt::Key_1)
        m_scene->frontView();
    else if(key == Qt::Key_N)
        m_state->toggleNormals();
    else if(key == Qt::Key_F1)
        m_scene->setCamera(Scene::Camera_Static);
    else if(key == Qt::Key_F2)
        m_scene->setCamera(Scene::Camera_Flying);
    else if(key == Qt::Key_F3)
        m_scene->setCamera(Scene::Camera_Jumping);
    else if(key == Qt::Key_S)
        m_scene->exportCurrentItem();
    else if(key == Qt::Key_Z)
        m_state->toggleWireframe();
    else if(key == Qt::Key_P)
        m_state->toggleProjection();
    else if(key == Qt::Key_Space)
        toggleAnimation();
    QGLWidget::keyReleaseEvent(e);
    update();
}

void SceneViewport::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();
    
    if(m_transState.active)
    {
        int dx = m_transState.x0 - x;
        int dy = m_transState.y0 - y;
        m_scene->delta().x = (m_transState.last.x - (dx / 100.0));
        m_scene->delta().y = (m_transState.last.y + (dy / 100.0));
        update();
    }
    
    if(m_rotState.active)
    {
        int dx = m_rotState.x0 - x;
        int dy = m_rotState.y0 - y;
        m_scene->theta().x = (m_rotState.last.x + (dy * 2.0));
        m_scene->theta().y = (m_rotState.last.y + (dx * 2.0));
        update();
    }
}

void SceneViewport::mousePressEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();
    if(e->button() & Qt::MiddleButton)       // middle button pans the scene
    {
        m_transState.active = true;
        m_transState.x0 = x;
        m_transState.y0 = y;
        m_transState.last = m_scene->delta();
    }
    else if(e->button() & Qt::LeftButton)   // left button rotates the scene
    {
        m_rotState.active = true;
        m_rotState.x0 = x;
        m_rotState.y0 = y;
        m_rotState.last = m_scene->theta();
		setFocus();
    }
    else
    {
        e->ignore();
        QGLWidget::mousePressEvent(e);
        return;
    }
    update();
}

void SceneViewport::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() & Qt::MiddleButton)
        m_transState.active = false;
    else if(e->button() & Qt::LeftButton)
        m_rotState.active = false;
    else
    {
        e->ignore();
        QGLWidget::mouseReleaseEvent(e);
        return;
    }
    update();
}

void SceneViewport::wheelEvent(QWheelEvent *e)
{
    // mouse wheel up zooms towards the scene
    // mouse wheel down zooms away from scene
    m_scene->sigma() *= pow(1.01, e->delta() / 8);
    update();
}
