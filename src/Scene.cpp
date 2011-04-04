#include "Scene.h"

Scene::Scene(QObject *parent) : QObject(parent)
{
}

Scene::~Scene()
{
}

void Scene::reset()
{
    emit invalidated();
}

bool Scene::load()
{
    reset();
    return true;
}

void Scene::loadTextures()
{
}

QVector3D Scene::orientation() const
{
    return QVector3D();
}

void Scene::animate()
{
}

QWidget * Scene::createControlWidget(QWidget * /* parent */)
{
    return 0;
}

void Scene::keyReleaseEvent(QKeyEvent *)
{
}
