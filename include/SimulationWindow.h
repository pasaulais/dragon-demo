#ifndef INITIALS_SIMULATION_WINDOW_H
#define INITIALS_SIMULATION_WINDOW_H

#include <QWidget>

class Scene;
class SceneViewport;

class SimulationWindow : public QWidget
{
public:
    SimulationWindow(Scene *scene, QWidget *parent = 0);

private:
    SceneViewport *m_viewport;
};

#endif
