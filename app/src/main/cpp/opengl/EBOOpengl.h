//
// Created by Fly on 2022/9/23.
//

#ifndef NDK_OPENGLES_LEARN_EBOOPENGL_H
#define NDK_OPENGLES_LEARN_EBOOPENGL_H
#include "BaseOpengl.h"

class EBOOpengl:public BaseOpengl {

public:
    EBOOpengl();
    virtual ~EBOOpengl();
    virtual void onDraw();
private:
    GLint positionHandle{-1};
    GLint colorHandle{-1};
    GLuint vbo{0};
    GLuint vao{0};
    GLuint ebo{0};
};


#endif //NDK_OPENGLES_LEARN_EBOOPENGL_H
