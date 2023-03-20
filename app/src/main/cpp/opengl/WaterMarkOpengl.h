//
// Created by Fly on 2023/2/21.
//

#ifndef NDK_OPENGLES_LEARN_WATERMARKOPENGL_H
#define NDK_OPENGLES_LEARN_WATERMARKOPENGL_H

#include "BaseOpengl.h"

class WaterMarkOpengl:public BaseOpengl {

public:
    WaterMarkOpengl();

    virtual ~WaterMarkOpengl();

    // override要么就都写，要么就都不写，不要一个虚函数写override，而另外一个虚函数不写override，不然可能编译不过
    virtual void onDraw() override;

    virtual void setPixel(void *data, int width, int height, int length) override;

    // 设置水印纹理数据
    virtual void setWaterPixel(void *data, int width, int height, int length) override;

private:
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLint textureSampler{-1};
    GLint waterTextureSampler{-1};
    GLuint textureId{0};
    GLuint waterTextureId{0};
    int imageWidth = 0;
    int imageHeight = 0;
};

#endif //NDK_OPENGLES_LEARN_WATERMARKOPENGL_H