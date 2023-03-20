//
// Created by Fly on 2022/8/25.
//

#ifndef NDK_OPENGLES_LEARN_TRIANGLEOPENGL_H
#define NDK_OPENGLES_LEARN_TRIANGLEOPENGL_H
#include "BaseOpengl.h"

class TriangleOpengl: public BaseOpengl{
public:
    TriangleOpengl();
    virtual ~TriangleOpengl();
    virtual void onDraw();

private:
    GLint positionHandle{-1};
    GLint colorHandle{-1};
};


#endif //NDK_OPENGLES_LEARN_TRIANGLEOPENGL_H
