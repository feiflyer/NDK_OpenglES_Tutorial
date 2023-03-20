#include <jni.h>
#include <string>
#include <cstdint>
#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "GLES3/gl3.h"
#include "opengl/TriangleOpengl.h"
#include "opengl/RectOpengl.h"
#include "opengl/TextureMapOpengl.h"
#include "utils/Log.h"
#include "android/bitmap.h"
#include "opengl/MatrixTransformOpengl.h"
#include "opengl/VBOVAOOpengl.h"
#include "opengl/EBOOpengl.h"
#include "opengl/FBOOpengl.h"
#include "opengl/PBOOpengl.h"
#include "opengl/YUVRenderOpengl.h"
#include "opengl/RGBtoYUVOpengl.h"
#include "opengl/WaterMarkOpengl.h"
#include "opengl/TextRenderOpengl.h"

static const int YUV_DATA_TYPE_NV12 = 0;
static const int YUV_DATA_TYPE_NV21 = 1;

static const int DRAW_TYPE_TRIANGLE = 0;
static const int DRAW_TYPE_RECT = 1;
static const int DRAW_TYPE_TEXTURE_MAP = 2;
static const int DRAW_TYPE_MATRIX_TRANSFORM = 3;
static const int DRAW_TYPE_VBO_VAO = 4;
static const int DRAW_TYPE_EBO_IBO = 5;
static const int DRAW_TYPE_FBO = 6;
static const int DRAW_TYPE_PBO = 7;
static const int DRAW_TYPE_YUV_RENDER = 8;
static const int DRAW_TYPE_RGB_TO_YUV = 9;
static const int DRAW_WATER_MARK = 10;
static const int DRAW_TEXT_RENDER = 11;
static const int DRAW_TEXT_ARRAY = 12;
static const int DRAW_MRT_RENDER = 13;
static const int DRAW_2D_LUT_RENDER = 14;
static const int DRAW_CUBE_LUT_RENDER = 15;

jlong eglHelperNativePtrInit(JNIEnv *env, jobject thiz) {
    EglHelper *eglHelper = new EglHelper();
    return reinterpret_cast<uintptr_t>(eglHelper);
}

void eglSurfaceCreated(JNIEnv *env, jobject thiz, jlong native_ptr, jobject surface) {
    if (native_ptr != 0) {
        EglHelper *eglHelper = reinterpret_cast<EglHelper *>(native_ptr);
        ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
        // todo ANativeWindow_release
        eglHelper->initEgl(nativeWindow);
    }
}

void eglSurfaceChanged(JNIEnv *env, jobject thiz, jlong native_ptr, jint width, jint height) {

    if (native_ptr != 0) {
        //设置视口大小
        glViewport(0, 0, width, height);
        // 绿色清屏
//        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        // 蓝色清屏
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        EglHelper *eglHelper = reinterpret_cast<EglHelper *>(native_ptr);
        eglHelper->viewWidth = width;
        eglHelper->viewHeight = height;

        eglHelper->swapBuffers();
    }
}

void eglSurfaceDestroyed(JNIEnv *env, jobject thiz, jlong native_ptr) {
    if (native_ptr != 0) {
        EglHelper *eglHelper = reinterpret_cast<EglHelper *>(native_ptr);
        eglHelper->destroyEgl();
        delete eglHelper;
    }
}

jlong gl_nativeInit(JNIEnv *env, jobject thiz, jlong egl_ptr, jint type) {
    if (egl_ptr != 0) {
        EglHelper *eglHelper = reinterpret_cast<EglHelper *>(egl_ptr);
        switch (type) {
            case DRAW_TYPE_TRIANGLE: {
                // 这里的大括号不能少
                TriangleOpengl *triangleOpengl = new TriangleOpengl();
                triangleOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(triangleOpengl);
            }

            case DRAW_TYPE_TEXTURE_MAP: {
                // 这里的大括号不能少
                TextureMapOpengl *textureMapOpengl = new TextureMapOpengl();
                textureMapOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(textureMapOpengl);
            }

            case DRAW_TYPE_RECT: {
                RectOpengl *rectOpengl = new RectOpengl();
                rectOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(rectOpengl);
            }

                // vbo/vao
            case DRAW_TYPE_VBO_VAO: {
                VBOVAOOpengl *vbovaoOpengl = new VBOVAOOpengl();
                vbovaoOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(vbovaoOpengl);
            }

                // ebo
            case DRAW_TYPE_EBO_IBO: {
                EBOOpengl *eboOpengl = new EBOOpengl();
                eboOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(eboOpengl);
            }

                // fbo
            case DRAW_TYPE_FBO: {
                FBOOpengl *fboOpengl = new FBOOpengl();
                fboOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(fboOpengl);
            }

            case DRAW_TYPE_PBO: {
                PBOOpengl *pboOpengl = new PBOOpengl();
                pboOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(pboOpengl);
            }

            case DRAW_TYPE_YUV_RENDER: {
                YUVRenderOpengl *yuvRenderOpengl = new YUVRenderOpengl();
                yuvRenderOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(yuvRenderOpengl);
            }

            case DRAW_TYPE_RGB_TO_YUV: {
                RGBtoYUVOpengl *rgBtoYuvOpengl = new RGBtoYUVOpengl();
                rgBtoYuvOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(rgBtoYuvOpengl);
            }

            case DRAW_TYPE_MATRIX_TRANSFORM: {
                MatrixTransformOpengl *matrixTransformOpengl = new MatrixTransformOpengl();
                matrixTransformOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(matrixTransformOpengl);
            }

            case DRAW_WATER_MARK: {
                WaterMarkOpengl *waterMarkOpengl = new WaterMarkOpengl();
                waterMarkOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(waterMarkOpengl);
            }

            case DRAW_TEXT_RENDER: {
                TextRenderOpengl *textRenderOpengl = new TextRenderOpengl();
                textRenderOpengl->eglHelper = eglHelper;
                return reinterpret_cast<jlong>(textRenderOpengl);
            }


        }
    }
    return 0;
}

void gl_free(JNIEnv *env, jobject thiz, jlong ptr, jint type) {
    if (ptr != 0) {
        BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
        delete baseOpengl;
    }
}

void onGlDraw(JNIEnv *env, jobject thiz, jlong native_ptr, jint type) {
    if (native_ptr != 0) {
        BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(native_ptr);
        baseOpengl->onDraw();
    }
}

void JNICALL onSetMvpMatrix(JNIEnv *env, jobject thiz, jlong ptr,
                                                              jfloatArray mvp) {
    LOGD("onSetMvpMatrix call")
    if (ptr != 0) {
        BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
        jint mvpLength = env->GetArrayLength(mvp);
        // 获取数组内容
        float *mvpData = reinterpret_cast<float *>(reinterpret_cast<uint8_t *>(env->GetFloatArrayElements(
                mvp, JNI_FALSE)));
        env->ReleaseFloatArrayElements(mvp,
                                       reinterpret_cast<jfloat *>(reinterpret_cast<jbyte *>(mvpData)), 0);
        baseOpengl->setMvpMatrix(mvpData);
    }
}

jbyteArray JNICALL onReadPixel(JNIEnv *env, jobject thiz, jlong ptr,
                               jint draw_type) {
    LOGD("onReadPixel call")
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    uint8_t *data = nullptr;
    int width = 0;
    int height = 0;
    baseOpengl->readPixel(&data, &width, &height);
    LOGD("onReadPixel call  width:%d", width);
    int size = width * height * 4;
    jbyteArray jbArray = env->NewByteArray(size);
    env->SetByteArrayRegion(jbArray, 0, size, reinterpret_cast<const jbyte *>(data));
    delete[] data;
    return jbArray;
}

jbyteArray JNICALL onReadYUV(JNIEnv *env, jobject thiz, jlong ptr,
                               jint draw_type) {
    LOGD("onReadYUV call")
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    uint8_t *data = nullptr;
    int width = 0;
    int height = 0;
    baseOpengl->readYUV(&data, &width, &height);
    LOGD("onReadYUV call  width:%d", width);
    int size = width * height * 1.5;
    jbyteArray jbArray = env->NewByteArray(size);
    env->SetByteArrayRegion(jbArray, 0, size, reinterpret_cast<const jbyte *>(data));
    delete[] data;
    return jbArray;
}

void onSetYuvData(JNIEnv *env, jobject thiz,jlong ptr, jbyteArray y_data, jbyteArray uv_data,
                  jint width, jint height,jint yuvType,
                  jint draw_type) {
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    jint y_length = env->GetArrayLength(y_data);
    jint uv_length = env->GetArrayLength(uv_data);
    LOGD("onSetYuvData y_length:%d,uv_length:%d",y_length,uv_length);
//    uint8_t *yPtrData = new uint8_t[y_length];
//    uint8_t *uvPtrData = new uint8_t[uv_length];
    // 获取数组内容
    uint8_t *yPtrData = reinterpret_cast<uint8_t *>(env->GetByteArrayElements(y_data, JNI_FALSE));
    uint8_t *uvPtrData = reinterpret_cast<uint8_t *>(env->GetByteArrayElements(uv_data, JNI_FALSE));
    baseOpengl->setYUVData(yPtrData,uvPtrData,width,height,yuvType);
    env->ReleaseByteArrayElements(y_data, reinterpret_cast<jbyte *>(yPtrData), 0);
    env->ReleaseByteArrayElements(uv_data, reinterpret_cast<jbyte *>(uvPtrData), 0);
}

void gl_setCubeLut(JNIEnv *env, jobject thiz, jlong ptr,
                   jint width, jint height, jint depth,
                   jobject buffer){
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    float *data = static_cast<float *>(env->GetDirectBufferAddress(buffer));
    // 获取buffer的容量
    jlong bufferCapacity  = env->GetDirectBufferCapacity(buffer);
    baseOpengl->setLutCubeData(width,height,depth,data);
}


void JNICALL gl_setBitmap(JNIEnv *env, jobject thiz, jlong ptr, jobject bitmap) {
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    AndroidBitmapInfo bitmapInfo;
    // 获取图片信息
    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    // 获取像素信息
    uint8_t *data = nullptr;
    AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&data));
    baseOpengl->setPixel(data, bitmapInfo.width, bitmapInfo.height,
                         bitmapInfo.stride * bitmapInfo.height);
    AndroidBitmap_unlockPixels(env, bitmap);
}

void JNICALL gl_setBitmap2(JNIEnv *env, jobject thiz, jlong ptr, jobject bitmap) {
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    AndroidBitmapInfo bitmapInfo;
    // 获取图片信息
    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    // 获取像素信息
    uint8_t *data = nullptr;
    AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&data));
    baseOpengl->setPixel2(data, bitmapInfo.width, bitmapInfo.height,
                         bitmapInfo.stride * bitmapInfo.height);
    AndroidBitmap_unlockPixels(env, bitmap);
}

void JNICALL gl_setLutBitmap(JNIEnv *env, jobject thiz, jlong ptr, jobject bitmap) {
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    AndroidBitmapInfo bitmapInfo;
    // 获取图片信息
    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    // 获取像素信息
    uint8_t *data = nullptr;
    AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&data));
    baseOpengl->setLutPixel(data, bitmapInfo.width, bitmapInfo.height,
                          bitmapInfo.stride * bitmapInfo.height);
    AndroidBitmap_unlockPixels(env, bitmap);
}


void JNICALL gl_setWaterBitmap(JNIEnv *env, jobject thiz, jlong ptr, jobject bitmap) {
    BaseOpengl *baseOpengl = reinterpret_cast<BaseOpengl *>(ptr);
    AndroidBitmapInfo bitmapInfo;
    // 获取图片信息
    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    // 获取像素信息
    uint8_t *data = nullptr;
    AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&data));
    baseOpengl->setWaterPixel(data, bitmapInfo.width, bitmapInfo.height,
                         bitmapInfo.stride * bitmapInfo.height);
    AndroidBitmap_unlockPixels(env, bitmap);
}


static JNINativeMethod nativeMethod_EGLHelper[] = {
        // Java中的函数名
        {"n_nativePtrInit",
                // 函数签名信息
                "()J",
                // native的函数指针
                (jlong *) (eglHelperNativePtrInit)},

        {"n_surfaceCreated",
                // 函数签名信息
                "(JLandroid/view/Surface;)V",
                // native的函数指针
                (void *) (eglSurfaceCreated)},

        {"n_surfaceChanged",
                // 函数签名信息
                "(JII)V",
                // native的函数指针
                (void *) (eglSurfaceChanged)},

        {"n_surfaceDestroyed",
                // 函数签名信息
                "(J)V",
                // native的函数指针
                (void *) (eglSurfaceDestroyed)},
};

static JNINativeMethod nativeMethod_base_opengl[] = {
        // Java中的函数名
        {"n_gl_nativeInit",
                // 函数签名信息
                "(JI)J",
                // native的函数指针
                (jlong *) (gl_nativeInit)},
        // Java中的函数名
        {"n_free",
                // 函数签名信息
                "(JI)V",
                // native的函数指针
                (void *) (gl_free)},
        {"n_setBitmap",
                // 函数签名信息
                "(JLandroid/graphics/Bitmap;)V",
                // native的函数指针
                (void *) (gl_setBitmap)},

        {"n_setLutBitmap",
                // 函数签名信息
                "(JLandroid/graphics/Bitmap;)V",
                // native的函数指针
                (void *) (gl_setLutBitmap)},

        {"n_setBitmap2",
                // 函数签名信息
                "(JLandroid/graphics/Bitmap;)V",
                // native的函数指针
                (void *) (gl_setBitmap2)},

        {"n_setWaterBitmap",
                // 函数签名信息
                "(JLandroid/graphics/Bitmap;)V",
                // native的函数指针
                (void *) (gl_setWaterBitmap)},
        {"n_onGlDraw",
                // 函数签名信息
                "(JI)V",
                // native的函数指针
                (void *) (onGlDraw)},
        {"n_readPixel",
                // 函数签名信息
                "(JI)[B",
                // native的函数指针
                (jbyteArray *) (onReadPixel)},
        {"n_setMvpMatrix",
                // 函数签名信息
                "(J[F)V",
                // native的函数指针
                (void *) (onSetMvpMatrix)},
        {"n_readYUV",
                // 函数签名信息
                "(JI)[B",
                // native的函数指针
                (jbyteArray *) (onReadYUV)},
        {"n_setYuvData",
                // 函数签名信息
                "(J[B[BIII)V",
                // native的函数指针
                (void *) (onSetYuvData)},

        {"n_setCubeLut",
                // 函数签名信息
                "(JIIILjava/nio/FloatBuffer;)V",
                // native的函数指针
                (void *) (gl_setCubeLut)},

};

static int
RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodNum) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

// 类库加载时自动调用
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reversed) {
    JNIEnv *env = NULL;
    // 初始化JNIEnv
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_FALSE;
    }
    // 动态注册
    RegisterNativeMethods(env, "com/fly/opengles/learn/egl/EGLHelper", nativeMethod_EGLHelper,
                          sizeof(nativeMethod_EGLHelper) / sizeof(JNINativeMethod));
    RegisterNativeMethods(env, "com/fly/opengles/learn/opengl/BaseOpengl", nativeMethod_base_opengl,
                          sizeof(nativeMethod_base_opengl) / sizeof(JNINativeMethod));

    // 返回JNI使用的版本
    return JNI_VERSION_1_6;
}