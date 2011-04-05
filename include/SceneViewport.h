#ifndef INITIALS_SCENE_VIEWPORT_H
#define INITIALS_SCENE_VIEWPORT_H

#include <ctime>
#include <QGLWidget>
#include <QVector3D>
#include <QVector4D>

class QTimer;
class QPainter;
class Scene;

typedef struct
{
    bool active;
    int x0;
    int y0;
    QVector3D last;        // value of delta/theta when the user last clicked
} MouseState;

class SceneViewport : public QGLWidget
{
    Q_OBJECT

public:
    SceneViewport(Scene *scene = 0, QWidget *parent = 0);
    virtual ~SceneViewport();

    Scene* scene() const;

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
    void setupGLState();
    void setupGLViewport(int w, int h);
    void restoreGLState();
    void paintFPS(QPainter *p, float fps);
    void startFPS();
    void updateAnimationState();
    void draw_axis();
    void draw_axes();
    void draw_axis_grids(bool draw_x, bool draw_y, bool draw_z);
    void draw_axis_grid();

    void reset_camera();
    void top_view();
    void side_view();
    void front_view();

    void toggle_animation();

    Scene *m_scene;
    QTimer *m_renderTimer;
    QColor m_bgColor;

    QVector4D m_ambient0;
    QVector4D m_diffuse0;
    QVector4D m_specular0;
    QVector4D m_light0_pos;

    // viewer settings
    QVector3D m_delta;
    QVector3D m_theta;
    float m_sigma;
    MouseState m_trans_state;
    MouseState m_rot_state;
    // on/off settings
    bool m_draw_axes;
    bool m_draw_grids;
    bool m_animate;
    bool m_wireframe_mode;
    bool m_projection_mode;
    // FPS settings
    QTimer *m_fpsTimer;
    clock_t m_start;
    uint m_frames;
    float m_lastFPS;
};

#endif
