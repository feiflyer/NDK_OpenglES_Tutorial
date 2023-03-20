//
// Created by Fly on 2022/8/25.
//

#ifndef NDK_OPENGLES_LEARN_BASEOPENGL_H
#define NDK_OPENGLES_LEARN_BASEOPENGL_H
#include "../eglhelper/EglHelper.h"
#include "GLES3/gl3.h"
#include <string>

class BaseOpengl {
public:
    EglHelper *eglHelper{nullptr};
    GLint program{0};
    virtual void readPixel(uint8_t **data,int *width,int *height);
    virtual void readYUV(uint8_t **data,int *width,int *height);
    virtual void setMvpMatrix(float *mvp);
    // 设置水印纹理数据
    virtual void setWaterPixel(void *data, int width, int height, int length);

public:
    BaseOpengl();
    // 析构函数必须是虚函数
    virtual ~BaseOpengl();
    // 加载着色器并链接成程序
    void initGlProgram(std::string ver,std::string fragment);
    // 绘制
    virtual void onDraw() = 0;
    // 设置纹理数据
    virtual void setPixel(void *data, int width, int height, int length);
    // 设置Lut滤镜、着色器固定了512X512的滤镜
    virtual void setLutPixel(void *data, int width, int height, int length);
    virtual void setPixel2(void *data, int width, int height, int length);
    // 设置yuv数据
    virtual void setYUVData(void *y_data,void *uv_data, int width, int height, int yuvType);

    virtual void setLutCubeData(int width,int height,int depth,float *data);

    void checkError(GLuint program);
};


#endif //NDK_OPENGLES_LEARN_BASEOPENGL_H
