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

#ifndef INITIALS_SCENE_VIEWPORT_H
#define INITIALS_SCENE_VIEWPORT_H

#include <QGLWidget>
#include <QDateTime>
#include "Vertex.h"

class QTimer;
class QPainter;
class QGLFormat;
class Scene;
class RenderState;

typedef struct
{
    bool active;
    int x0;
    int y0;
    vec3 last;        // value of delta/theta when the user last clicked
} MouseState;

class SceneViewport : public QGLWidget
{
    Q_OBJECT

public:
    SceneViewport(Scene *scene, RenderState *state, const QGLFormat &format, QWidget *parent = 0);
    virtual ~SceneViewport();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void paintEvent(QPaintEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void wheelEvent(QWheelEvent *e);

private slots:
    void updateFPS();
    void animateScene();

private:
    void paintFPS(QPainter *p, float fps);
    void startFPS();
    void updateAnimationState();
    void toggleAnimation();
    void resetCamera();

    Scene *m_scene;
    RenderState *m_state;
    QTimer *m_renderTimer;

    // viewer settings
    MouseState m_transState;
    MouseState m_rotState;
    bool m_animate;

    // FPS settings
    QTimer *m_fpsTimer;
    QDateTime m_start;
    uint m_frames;
    float m_lastFPS;
};

#endif
