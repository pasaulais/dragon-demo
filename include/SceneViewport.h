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
    SceneViewport(const QGLFormat &format, QWidget *parent = 0);
    virtual ~SceneViewport();

    RenderState *state() const;

    Scene* scene() const;
    void setScene(Scene *newScene);

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

private:
    void paintFPS(QPainter *p, float fps);
    void startFPS();
    void updateAnimationState();
    void toggleAnimation();
    void resetCamera();
    void topView();
    void sideView();
    void frontView();

    Scene *m_scene;
    RenderState *m_state;
    QTimer *m_renderTimer;

    // viewer settings
    vec3 m_delta;
    vec3 m_theta;
    float m_sigma;
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
