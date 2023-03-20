//
// Created by Fly on 2022/9/6.
//

#ifndef NDK_OPENGLES_LEARN_RECTOPENGL_H
#define NDK_OPENGLES_LEARN_RECTOPENGL_H
#include "BaseOpengl.h"

class RectOpengl:public BaseOpengl {
public:
    RectOpengl();
    virtual ~RectOpengl();
    virtual void onDraw();
private:
    GLint positionHandle{-1};
    GLint colorHandle{-1};
};

#endif //NDK_OPENGLES_LEARN_RECTOPENGL_H
