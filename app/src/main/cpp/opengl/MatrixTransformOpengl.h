//
// Created by Fly on 2022/9/19.
//

#ifndef NDK_OPENGLES_LEARN_MATRIXTRANSFORMOPENGL_H
#define NDK_OPENGLES_LEARN_MATRIXTRANSFORMOPENGL_H

#include "BaseOpengl.h"

class MatrixTransformOpengl:public BaseOpengl {
public:
    MatrixTransformOpengl();
    virtual ~MatrixTransformOpengl();
    virtual void onDraw() override;
    virtual void setMvpMatrix(float *mvp) override;
    virtual void setPixel(void *data, int width, int height, int length) override;
private:
    float mvpMatrix[16];
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLint textureSampler{-1};
    GLint matrixHandle{-1};
    GLuint textureId{0};
    int imageWidth{0};
    int imageHeight{0};

};


#endif //NDK_OPENGLES_LEARN_MATRIXTRANSFORMOPENGL_H
