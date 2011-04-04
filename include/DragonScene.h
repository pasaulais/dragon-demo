#ifndef INITIALS_DRAGON_SCENE_H
#define INITIALS_DRAGON_SCENE_H

#include <QObject>
#include <QList>
#include "Scene.h"

class Mesh;
class Dragon;

class DragonScene : public Scene
{
    Q_OBJECT

public:
    DragonScene(QObject *parent = 0);
    virtual ~DragonScene();

    virtual bool load();
    bool meshLoaded() const;

    virtual void draw();
    void drawMesh(Mesh *m);

    void select_next();
    void select_previous();

    virtual void keyReleaseEvent(QKeyEvent *e);

    virtual void reset();

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

public slots:
    void animate();

private:
    Mesh * loadMesh(QString path);
    void drawDragonHoldingA(Dragon *d);
    void drawDragonHoldingP(Dragon *d);
    void drawDragonHoldingS(Dragon *d);
    static float sawtooth(float t);
    static float spaced_rect(float t, float w, float a);
    static float spaced_sawtooth(float x, float w, float a);
    static float spaced_cos(float x, float w, float a);
    void animateDragon(Dragon *d, float t);

    int m_loadedMeshes, m_missingMeshes;
    bool m_drawNormals;
    int m_selected;
    int m_detailLevel;
    QList<Dragon *> m_dragons;
};

#endif
