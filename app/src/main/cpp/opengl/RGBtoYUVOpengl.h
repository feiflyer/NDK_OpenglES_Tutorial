//
// Created by Fly on 2022/12/14.
//

#ifndef NDK_OPENGLES_LEARN_RGBTOYUVOPENGL_H
#define NDK_OPENGLES_LEARN_RGBTOYUVOPENGL_H

#include "BaseOpengl.h"

class RGBtoYUVOpengl:public BaseOpengl{

public:
    RGBtoYUVOpengl();

    virtual ~RGBtoYUVOpengl();

    virtual void onDraw() override;

    virtual void setPixel(void *data, int width, int height, int length) override;
    virtual void readYUV(uint8_t **data,int *width,int *height) override;

private:
    void fboPrepare();
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLint textureSampler{-1};
    GLuint textureId{0};
    GLuint u_Offset{0};
    // fbo纹理id
    GLuint fboTextureId{0};
    // fbo
    GLuint fboId{0};
    int imageWidth{0};
    int imageHeight{0};
};


#endif //NDK_OPENGLES_LEARN_RGBTOYUVOPENGL_H
