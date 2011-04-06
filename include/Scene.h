#ifndef INITIALS_SCENE_H
#define INITIALS_SCENE_H

#include <QObject>
#include <QVector3D>
#include <QList>

class QKeyEvent;
class RenderState;
class Dragon;

class Scene : public QObject
{
    Q_OBJECT

public:
    Scene(RenderState *state, QObject *parent = 0);

    bool load();
    void loadTextures();
    void freeTextures();

    QVector3D orientation() const;

    void draw();

    void select_next();
    void select_previous();

    virtual void keyReleaseEvent(QKeyEvent *e);

    void reset();

    static float sawtooth(float t);
    static float spaced_rect(float t, float w, float a);
    static float spaced_sawtooth(float x, float w, float a);
    static float spaced_cos(float x, float w, float a);

    enum Camera
    {
        Camera_Static,
        Camera_Jumping,
        Camera_Flying
    };

    enum Item
    {
        SCENE,
        LETTER_P,
        LETTER_A,
        LETTER_S,
        DRAGON,
        DRAGON_UPPER,
        DRAGON_HEAD,
        DRAGON_TONGUE,
        DRAGON_JOINT,
        DRAGON_BODY,
        DRAGON_CHEST,
        DRAGON_PAWS,
        DRAGON_PAW,
        DRAGON_WING,
        DRAGON_WING_OUTER,
        DRAGON_WING_PART,
        DRAGON_WING_MEMBRANE,
        DRAGON_TAIL,
        DRAGON_TAIL_END,
        LAST = DRAGON_TAIL_END
    };

    void exportItem(Item item, QString path);

signals:
    void invalidated();

public slots:
    void animate();

private:
    void drawItem(Item item);
    void drawScene();
    void drawFloor();
    void drawDragonHoldingA(Dragon *d);
    void drawDragonHoldingP(Dragon *d);
    void drawDragonHoldingS(Dragon *d);
    static QString itemText(Item item);

    RenderState *m_state;
    int m_selected;
    int m_detailLevel;
    Camera m_camera;
    QVector3D m_theta;
    Dragon *m_debugDragon;
    QList<Dragon *> m_dragons;
    bool m_exportQueued;
};

#endif
