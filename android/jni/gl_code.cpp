#ifdef JNI_WRAPPER
#include <jni.h>
#include <android/log.h>

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../include/RenderState.h"
#include "../../include/RenderStateGL1.h"
#include "../../include/Scene.h"

#define  LOG_TAG    "GLinitials"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void checkGlError(const char* op)
{
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static RenderStateGL1 *state = 0;
static Scene *scene = 0;
static int width = 0, height = 0;

static bool init(int w, int h)
{
    state = new RenderStateGL1();
    scene = new Scene(state);
    state->setupViewport(w, h);
    width = w;
    height = h;
    return true;
}

static void dispose()
{
    delete scene;
    delete state;
}

static void renderFrame()
{
    if(!state)
        return;
    state->beginFrame(width, height);
    scene->draw();
    state->endFrame();
}

extern "C"
{
    JNIEXPORT void JNICALL Java_fr_free_pasaulais_glinitials_GLInitialsLib_init(JNIEnv * env, jclass obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_fr_free_pasaulais_glinitials_GLInitialsLib_render(JNIEnv * env, jclass obj);
    JNIEXPORT void JNICALL Java_fr_free_pasaulais_glinitials_GLInitialsLib_dispose(JNIEnv * env, jclass obj);
};

JNIEXPORT void JNICALL Java_fr_free_pasaulais_glinitials_GLInitialsLib_init(JNIEnv * env, jclass obj,  jint width, jint height)
{
    init(width, height);
}

JNIEXPORT void JNICALL Java_fr_free_pasaulais_glinitials_GLInitialsLib_render(JNIEnv * env, jclass obj)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_fr_free_pasaulais_glinitials_GLInitialsLib_dispose(JNIEnv * env, jclass obj)
{
    dispose();
}
#endif
