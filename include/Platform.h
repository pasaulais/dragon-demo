#ifndef INITIALS_PLATFORM_H
#define INITIALS_PLATFORM_H

#ifdef JNI_WRAPPER
#include <GLES/gl.h>
#include <android/log.h>
#define  LOG_TAG    "GLinitials"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#define  LOGI(...) fprintf(stderr, __VA_ARGS__);
#define  LOGE(...) fprintf(stderr, __VA_ARGS__);
#endif

#endif
