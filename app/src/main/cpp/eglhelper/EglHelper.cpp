//
// Created by Fly on 2022/8/24.
//

#include "EglHelper.h"
#include "../utils/Log.h"

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/5QBYGxdmQbXhWo6iqGbHbw
 *
 */
EglHelper::EglHelper() {

    mEglDisplay = EGL_NO_DISPLAY;
    mEglSurface = EGL_NO_SURFACE;
    mEglContext = EGL_NO_CONTEXT;
    mEglConfig = NULL;
    windowType = nullptr;
}

EglHelper::~EglHelper() {
    destroyEgl();
}

int EglHelper::initEgl(EGLNativeWindowType window) {

    windowType = window;
    //1、获取显示设备
    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(mEglDisplay == EGL_NO_DISPLAY)
    {
        LOGE("eglGetDisplay error");
        return -1;
    }
    // 2、 EGL初始化
    EGLint *version = new EGLint[2];
    if(!eglInitialize(mEglDisplay, &version[0], &version[1]))
    {
        LOGE("eglInitialize error");
        return -1;
    }

    //3、 资源配置，例如颜色位数等
    // 注意 EGL_OPENGL_ES3_BIT_KHR和EGL_NONE,0,EGL_NONE 不然可能会偶发的崩溃
    const EGLint attribs[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_STENCIL_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_NONE,0,
            EGL_NONE
    };


    EGLint num_config;
    // config_size传递1，适配性，为了防止有些机型返回多个config导致渲染异常
    if(!eglChooseConfig(mEglDisplay, attribs, NULL, 1, &num_config))
    {
        LOGE("eglChooseConfig  error 1");
        return -1;
    }

    //4、ChooseConfig
    if(!eglChooseConfig(mEglDisplay, attribs, &mEglConfig, num_config, &num_config))
    {
        LOGE("eglChooseConfig  error 2");
        return -1;
    }

    // 5、创建上下文，gles3版本
    int attrib_list[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };

    mEglContext = eglCreateContext(mEglDisplay, mEglConfig, EGL_NO_CONTEXT, attrib_list);

    if(mEglContext == EGL_NO_CONTEXT)
    {
        LOGE("eglCreateContext  error");
        return -1;
    }

    //6、创建渲染的Surface
    mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, window, NULL);
    if(mEglSurface == EGL_NO_SURFACE)
    {
        LOGE("eglCreateWindowSurface  error");
        return -1;
    }

    // 7、链接使用
    if(!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext))
    {
        LOGE("eglMakeCurrent  error");
        return -1;
    }
    LOGD("egl init success! ");
    return 0;
}

int EglHelper::swapBuffers() {

    if(mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE)
    {
        if(eglSwapBuffers(mEglDisplay, mEglSurface))
        {
            return 0;
        }
    }
    return -1;
}

void EglHelper::destroyEgl() {

    if(mEglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    if(mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(mEglDisplay, mEglSurface);
        mEglSurface = EGL_NO_SURFACE;
    }
    if(mEglDisplay != EGL_NO_DISPLAY && mEglContext != EGL_NO_CONTEXT){
        eglDestroyContext(mEglDisplay, mEglContext);
        mEglContext = EGL_NO_CONTEXT;
    }
    if(mEglDisplay != EGL_NO_DISPLAY)
    {
        eglTerminate(mEglDisplay);
        mEglDisplay = EGL_NO_DISPLAY;
    }
}
