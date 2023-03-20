//
// Created by Fly on 2022/11/1.
//

#ifndef NDK_OPENGLES_LEARN_YUVRENDEROPENGL_H
#define NDK_OPENGLES_LEARN_YUVRENDEROPENGL_H
#include "BaseOpengl.h"

class YUVRenderOpengl: public BaseOpengl{

public:
    YUVRenderOpengl();

    virtual ~YUVRenderOpengl();

    virtual void onDraw() override;

    // 设置yuv数据
    virtual void setYUVData(void *y_data,void *uv_data, int width, int height, int yuvType) override;

private:
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLint y_textureSampler{-1};
    GLint uv_textureSampler{-1};
    GLuint y_textureId{0};
    GLuint uv_textureId{0};
};


#endif //NDK_OPENGLES_LEARN_YUVRENDEROPENGL_H
