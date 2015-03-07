#include <QApplication>
#include <QGLFormat>
#include <QMessageBox>
#include "SceneViewport.h"
#include "Scene.h"
#include "RenderState.h"
#include "RenderStateGL1.h"
#include "RenderStateGL2.h"

bool loadResources(RenderState *state)
{
    state->loadMeshFromFile("floor", "meshes/floor.obj");
    state->loadMeshFromFile("letter_p", "meshes/LETTER_P.obj");
    state->loadMeshFromFile("letter_a", "meshes/LETTER_A.obj");
    state->loadMeshFromFile("letter_s", "meshes/LETTER_S.obj");
    state->loadMeshFromFile("wing_membrane", "meshes/dragon_wing_membrane.obj");
    state->loadMeshFromFile("joint", "meshes/dragon_joint_spin.obj");
    state->loadMeshFromFile("dragon_chest", "meshes/dragon_chest.obj");
    state->loadMeshFromFile("dragon_head", "meshes/dragon_head.obj");
    state->loadMeshFromFile("dragon_tail_end", "meshes/dragon_tail_end.obj");
    state->loadTextureFromFile("lava_green", "lava_green.tiff", true);
    state->loadTextureFromFile("scale_gold", "scale_green.tiff");
    state->loadTextureFromFile("scale_black", "scale_black.tiff");
    state->loadTextureFromFile("scale_bronze", "scale_bronze.tiff");
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

    // create the scene
    RenderStateGL2 state;
    Scene scene(&state);

    // create viewport for rendering the scene
    SceneViewport w(&scene, &state, f);
    w.setWindowState(Qt::WindowMaximized);
    w.setWindowTitle("Dragons Demo");
    w.makeCurrent();
    if(!loadResources(&state))
    {
        QMessageBox::critical(0, "Error", "Could not load the mesh files (they should be in the 'meshes' sub-directory).");
        return 1;
    }
    w.show();
    
    // main window loop
    app.exec();
    return 0;
}
