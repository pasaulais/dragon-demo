#ifndef INITIALS_SCENE_H
#define INITIALS_SCENE_H

#include <QObject>

class QWidget;
class QKeyEvent;

class Scene : public QObject
{
    Q_OBJECT

public:
    Scene(QObject *parent = 0);
    virtual ~Scene();

    virtual bool load();
    virtual void draw() = 0;
    virtual void reset();

    virtual QWidget * createControlWidget(QWidget *parent = 0);
    virtual void keyReleaseEvent(QKeyEvent *e);

public slots:
    virtual void animate();

signals:
    void invalidated();
};

#endif
