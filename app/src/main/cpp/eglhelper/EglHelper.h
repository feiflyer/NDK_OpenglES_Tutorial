//
// Created by Fly on 2022/8/24.
//

#ifndef NDK_OPENGLES_LEARN_EGLHELPER_H
#define NDK_OPENGLES_LEARN_EGLHELPER_H

#include "EGL/egl.h"
#include "EGL/eglext.h"

class EglHelper {

public:
    EGLDisplay  mEglDisplay;
    EGLSurface  mEglSurface;
    EGLConfig  mEglConfig;
    EGLContext mEglContext;
    EGLNativeWindowType windowType;
    int viewWidth{0};
    int viewHeight{0};

public:
    EglHelper();
    ~EglHelper();
    int initEgl(EGLNativeWindowType win);
    int swapBuffers();
    void destroyEgl();
};


#endif //NDK_OPENGLES_LEARN_EGLHELPER_H
