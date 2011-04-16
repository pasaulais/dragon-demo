LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := glinitials_jni
LOCAL_CFLAGS    := -Werror -DJNI_WRAPPER \
                -I/opt/android-ndk/sources/cxx-stl/stlport/stlport \
                -I../../include \
                -I../../tiff-3.8.2-1/include
LOCAL_SRC_FILES := gl_code.cpp ../../src/RenderState.cpp ../../src/RenderStateGL1.cpp \
                ../../src/Mesh.cpp  ../../src/MeshGL1.cpp ../../src/Material.cpp \
                ../../src/Vertex.cpp ../../src/Scene.cpp ../../src/Dragon.cpp
LOCAL_LDLIBS    := -llog -lGLESv1_CM \
                -L/opt/android-ndk/sources/cxx-stl/stlport/libs/armeabi -lstlport_static \
                -L../../tiff-3.8.2-1/armeabi -ltiff -ltiffdecoder
LOCAL_SHARED_LIBRARIES := libtiff litiffdecoder
APP_STL         := stlport_static

include $(BUILD_SHARED_LIBRARY)
