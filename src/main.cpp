#include <QApplication>
#include <QGLFormat>
#include <QMessageBox>
#include "SceneViewport.h"
#include "Scene.h"
#include "RenderState.h"
#include "RenderStateGL1.h"
#include "RenderStateGL2.h"

bool loadMeshes(RenderState *state)
{
    state->loadMeshObj("floor", "meshes/floor.obj");
    state->loadMeshObj("letter_p", "meshes/LETTER_P.obj");
    state->loadMeshObj("letter_a", "meshes/LETTER_A.obj");
    state->loadMeshObj("letter_s", "meshes/LETTER_S.obj");
    state->loadMeshObj("wing_membrane", "meshes/dragon_wing_membrane.obj");
    state->loadMeshObj("joint", "meshes/dragon_joint_spin.obj");
    state->loadMeshObj("dragon_chest", "meshes/dragon_chest.obj");
    state->loadMeshObj("dragon_head", "meshes/dragon_head.obj");
    state->loadMeshObj("dragon_tail_end", "meshes/dragon_tail_end.obj");
    return state->meshes().size() > 0;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    // define OpenGL options
    QGLFormat f;
    f.setAlpha(true);
    f.setSampleBuffers(true);
    f.setSwapInterval(0);

    // create and load the scene
    RenderStateGL2 state;
    Scene scene(&state);
    if(!loadMeshes(&state))
    {
        QMessageBox::critical(0, "Error", "Could not load the mesh files (they should be in the 'meshes' sub-directory).");
        return 1;
    }

    // create viewport for rendering the scene
    SceneViewport w(&scene, &state, f);
    w.setWindowState(Qt::WindowMaximized);
    w.setWindowTitle("Dragons Demo");
    w.show();
    
    // main window loop
    app.exec();
    return 0;
}
