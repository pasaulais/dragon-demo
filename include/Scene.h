#ifndef INITIALS_SCENE_H
#define INITIALS_SCENE_H

#include <QList>
#include <QDateTime>
#include "RenderState.h"
#include "Vertex.h"

class QKeyEvent;
class Dragon;

class Scene : public StateObject
{
public:
    Scene(RenderState *state);
    virtual ~Scene();

    bool load();
    void loadTextures();
    void freeTextures();

    vec3 & theta();
    float & sigma();
    vec3 & delta();

    void draw();

    void selectNext();
    void selectPrevious();

    void topView();
    void sideView();
    void frontView();

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

    void exportItem(Item item, string path);

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
    static string itemText(Item item);

    QDateTime m_started;
    int m_selected;
    int m_detailLevel;
    vec3 m_delta;
    vec3 m_theta;
    float m_sigma;
    Camera m_camera;
    vec3 m_thetaCamera;
    Dragon *m_debugDragon;
    QList<Dragon *> m_dragons;
    bool m_exportQueued;
};

#endif
