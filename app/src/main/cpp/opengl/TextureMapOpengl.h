//
// Created by Fly on 2022/9/5.
//

#ifndef NDK_OPENGLES_LEARN_TEXTUREMAPOPENGL_H
#define NDK_OPENGLES_LEARN_TEXTUREMAPOPENGL_H

#include "BaseOpengl.h"

class TextureMapOpengl : public BaseOpengl {
public:
    TextureMapOpengl();

    virtual ~TextureMapOpengl();

    virtual void onDraw() override;

    virtual void setPixel(void *data, int width, int height, int length) override;

private:
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLint textureSampler{-1};
    GLuint textureId{0};
};


#endif //NDK_OPENGLES_LEARN_TEXTUREMAPOPENGL_H
