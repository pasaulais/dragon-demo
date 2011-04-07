#include <cmath>
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
#include "RenderStateGL1.h"

SceneViewport::SceneViewport(const QGLFormat &format, QWidget *parent) : QGLWidget(format, parent)
{
    setMinimumSize(640, 480);
    m_scene = 0;
    m_state = new RenderStateGL1(this);
    m_renderTimer = new QTimer(this);
    m_renderTimer->setInterval(1000 / 60);
    m_start = 0;
    m_frames = 0;
    m_lastFPS = 0;
    m_fpsTimer = new QTimer(this);
    m_fpsTimer->setInterval(1000 / 6);
    setAutoFillBackground(false);
    connect(m_fpsTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
}

SceneViewport::~SceneViewport()
{
    if(m_scene)
        m_scene->freeTextures();
}

RenderState *SceneViewport::state() const
{
    return m_state;
}

Scene* SceneViewport::scene() const
{
    return m_scene;
}

void SceneViewport::setScene(Scene *newScene)
{
    if(m_scene)
    {
        m_scene->freeTextures();
        disconnect(m_scene, SIGNAL(invalidated()), this, SLOT(update()));
        disconnect(m_renderTimer, SIGNAL(timeout()), m_scene, SLOT(animate()));
    }
    m_scene = newScene;
    if(newScene)
    {
        if(context())
        {
            makeCurrent();
            newScene->loadTextures();
        }
        connect(newScene, SIGNAL(invalidated()), this, SLOT(update()));
        connect(m_renderTimer, SIGNAL(timeout()), newScene, SLOT(animate()));
    }
    update();
}

void SceneViewport::initializeGL()
{
    if(m_scene)
        m_scene->loadTextures();
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
    vec3 rot = m_theta;
    if(m_scene)
        rot = rot + m_scene->orientation();
    m_state->translate(m_delta.x, m_delta.y, m_delta.z);
    m_state->rotate(rot.x, 1.0, 0.0, 0.0);
    m_state->rotate(rot.y, 0.0, 1.0, 0.0);
    m_state->rotate(rot.z, 0.0, 0.0, 1.0);
    m_state->scale(m_sigma, m_sigma, m_sigma);
    if(m_scene)
        m_scene->draw();
    m_state->endFrame();
}

void SceneViewport::resetCamera()
{
    m_delta = vec3(-0.0, -0.5, -5.0);
    m_theta = vec3(21.0, -37.0, 0.0);
    m_transState.last = vec3();
    m_rotState.last = vec3();
    m_transState.active = false;
    m_rotState.active = false;
    m_sigma = 0.40;
    m_animate = true;
    m_state->reset();
    if(m_scene)
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

void SceneViewport::topView()
{
    m_theta = vec3(0.0, 0.0, 0.0);
}

void SceneViewport::sideView()
{
    m_theta = vec3(-90.0, 0.0, -90.0);
}

void SceneViewport::frontView()
{
    m_theta = vec3(-90.0, 0.0, 0.0);
}

void SceneViewport::startFPS()
{
    m_start = clock();
}

void SceneViewport::updateFPS()
{
    clock_t elapsed = clock() - m_start;
    m_lastFPS = m_frames / ((float)elapsed / CLOCKS_PER_SEC);
    m_frames = 0;
    m_start = clock();
}

void SceneViewport::paintFPS(QPainter *p, float fps)
{
    QFont f;
    f.setPointSizeF(16.0);
    f.setWeight(QFont::Bold);
    p->setFont(f);
    QString text = QString("%1 FPS").arg(fps, 0, 'g', 3);
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

void SceneViewport::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key == Qt::Key_Q)
        m_theta.y = (m_theta.y + 5.0);
    else if(key == Qt::Key_D)
        m_theta.y = (m_theta.y - 5.0);
    else if(key == Qt::Key_2)
        m_theta.x = (m_theta.x + 5.0);
    else if(key == Qt::Key_8)
        m_theta.x = (m_theta.x - 5.0);
    else if(key == Qt::Key_4)
        m_theta.z = (m_theta.z + 5.0);
    else if(key == Qt::Key_6)
        m_theta.z = (m_theta.z - 5.0);
    else if(key == Qt::Key_R)
        resetCamera();
    else if(key == Qt::Key_Z)
        m_state->toggleWireframe();
    else if(key == Qt::Key_P)
        m_state->toggleProjection();
    else if(key == Qt::Key_Space)
        toggleAnimation();
	else if(key == Qt::Key_7)
        topView();
	else if(key == Qt::Key_3)
        sideView();
	else if(key == Qt::Key_1)
        frontView();
    else if(m_scene)
        m_scene->keyReleaseEvent(e);
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
        m_delta.x = (m_transState.last.x - (dx / 100.0));
        m_delta.y = (m_transState.last.y + (dy / 100.0));
        update();
    }
    
    if(m_rotState.active)
    {
        int dx = m_rotState.x0 - x;
        int dy = m_rotState.y0 - y;
        m_theta.x = (m_rotState.last.x + (dy * 2.0));
        m_theta.y = (m_rotState.last.y + (dx * 2.0));
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
        m_transState.last = m_delta;
    }
    else if(e->button() & Qt::LeftButton)   // left button rotates the scene
    {
        m_rotState.active = true;
        m_rotState.x0 = x;
        m_rotState.y0 = y;
        m_rotState.last = m_theta;
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
    m_sigma *= pow(1.01, e->delta() / 8);
    update();
}
