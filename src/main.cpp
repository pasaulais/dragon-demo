#include <QApplication>
#include <QGLFormat>
#include <QMessageBox>
#include "SimulationWindow.h"
#include "DragonScene.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    // define OpenGL options
    QGLFormat f;
    f.setAlpha(true);
    f.setSampleBuffers(true);
    QGLFormat::setDefaultFormat(f);

    // load the scene
    DragonScene scene;
    if(!scene.load())
    {
        QMessageBox::critical(0, "Error", "Could not load the mesh files (they should be in the 'meshes' sub-directory).");
        return 1;
    }

    // create viewport for animating the robot
    SimulationWindow w(&scene);
    w.setWindowState(Qt::WindowMaximized);
    w.setWindowTitle("Dragons Demo");
    w.show();
    
    // main window loop
    app.exec();
    return 0;
}
