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
#include "RenderStateGL2.h"

SceneViewport::SceneViewport(const QGLFormat &format, QWidget *parent) : QGLWidget(format, parent)
{
    setMinimumSize(640, 480);
    m_scene = 0;
    m_state = new RenderStateGL2();
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
    if(m_scene)
        m_scene->freeTextures();
    delete m_state;
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
        m_scene->freeTextures();
    m_scene = newScene;
    if(newScene && context())
    {
        makeCurrent();
        newScene->loadTextures();
    }
    update();
}

void SceneViewport::initializeGL()
{
    m_state->init();
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
    if(m_scene)
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
    if(m_scene)
    {
        m_scene->animate();
        update();
    }
}

void SceneViewport::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key == Qt::Key_R)
        resetCamera();
    else if(key == Qt::Key_Z)
        m_state->toggleWireframe();
    else if(key == Qt::Key_P)
        m_state->toggleProjection();
    else if(key == Qt::Key_Space)
        toggleAnimation();
    else if(m_scene)
        m_scene->keyReleaseEvent(e);
    QGLWidget::keyReleaseEvent(e);
    update();
}

void SceneViewport::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();
    
    if(m_transState.active && m_scene)
    {
        int dx = m_transState.x0 - x;
        int dy = m_transState.y0 - y;
        m_scene->delta().x = (m_transState.last.x - (dx / 100.0));
        m_scene->delta().y = (m_transState.last.y + (dy / 100.0));
        update();
    }
    
    if(m_rotState.active && m_scene)
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
        m_transState.last = m_scene ? m_scene->delta() : vec3();
    }
    else if(e->button() & Qt::LeftButton)   // left button rotates the scene
    {
        m_rotState.active = true;
        m_rotState.x0 = x;
        m_rotState.y0 = y;
        m_rotState.last = m_scene ? m_scene->theta() : vec3();
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
    if(m_scene)
    {
        m_scene->sigma() *= pow(1.01, e->delta() / 8);
        update();
    }
}
