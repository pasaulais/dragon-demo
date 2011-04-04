#include <QHBoxLayout>
#include "SimulationWindow.h"
#include "SceneViewport.h"
#include "Scene.h"

SimulationWindow::SimulationWindow(Scene *scene, QWidget *parent) : QWidget(parent)
{
    m_viewport = new SceneViewport(scene);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_viewport);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_viewport->setFocus();
}
