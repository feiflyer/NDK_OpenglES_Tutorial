//
// Created by Fly on 2022/9/22.
//

#ifndef NDK_OPENGLES_LEARN_VBOVAOOPENGL_H
#define NDK_OPENGLES_LEARN_VBOVAOOPENGL_H
#include "BaseOpengl.h"

static const int NUM_VBO = 3;

class VBOVAOOpengl:public BaseOpengl{

public:
    VBOVAOOpengl();
    virtual ~VBOVAOOpengl();
    virtual void onDraw();
private:
    GLint positionHandle{-1};
    GLint colorHandle{-1};
    GLuint vbo[NUM_VBO];
    GLuint vao{0};

};


#endif //NDK_OPENGLES_LEARN_VBOVAOOPENGL_H
