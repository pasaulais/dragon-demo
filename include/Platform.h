#ifndef INITIALS_PLATFORM_H
#define INITIALS_PLATFORM_H

#include <string>

#ifdef JNI_WRAPPER
#include <GLES/gl.h>
#include <android/log.h>
#define  LOG_TAG    "GLinitials"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#include <GL/glew.h>
#include <cstdio>
#define  LOGI(...) fprintf(stderr, __VA_ARGS__);
#define  LOGE(...) fprintf(stderr, __VA_ARGS__);
#endif

void checkGlError(const char* op);
char *loadFileData(std::string path);
void freeFileData(char *data);

#endif
