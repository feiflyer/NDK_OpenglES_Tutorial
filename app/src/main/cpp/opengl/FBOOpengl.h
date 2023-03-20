//
// Created by Fly on 2022/9/26.
//

#ifndef NDK_OPENGLES_LEARN_FBOOPENGL_H
#define NDK_OPENGLES_LEARN_FBOOPENGL_H
#include "BaseOpengl.h"

class FBOOpengl:public BaseOpengl{

public:
    FBOOpengl();
    void onFboDraw();
    virtual ~FBOOpengl();
    // override要么就都写，要么就都不写，不要一个虚函数写override，而另外一个虚函数不写override，不然可能编译不过
    virtual void onDraw() override;
    virtual void setPixel(void *data, int width, int height, int length) override;
private:
    void fboPrepare();
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLuint vbo{0};
    GLuint vao{0};
    GLuint ebo{0};
    // 本身图像纹理id
    GLuint imageTextureId{0};
    // fbo纹理id
    GLuint fboTextureId{0};
    GLint textureSampler{-1};
    GLuint fboId{0};
    // 用于fbo的vbo和vao  也可以用数组的形式，这里为了方便理解先独立开来
    GLuint fboVbo{0};
    GLuint fboVao{0};
    int imageWidth{0};
    int imageHeight{0};
};

#endif //NDK_OPENGLES_LEARN_FBOOPENGL_H
