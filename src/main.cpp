// Copyright (c) 2009-2015, Pierre-Andre Saulais <pasaulais@free.fr>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
