//
// Created by Fly on 2022/10/25.
//

#ifndef NDK_OPENGLES_LEARN_PBOOPENGL_H
#define NDK_OPENGLES_LEARN_PBOOPENGL_H

#include "BaseOpengl.h"

static const int NUM_PBO = 2;

class PBOOpengl: public BaseOpengl{

public:
    PBOOpengl();
    virtual ~PBOOpengl();
    // override要么就都写，要么就都不写，不要一个虚函数写override，而另外一个虚函数不写override，不然可能编译不过
    virtual void onDraw() override;
    virtual void setPixel(void *data, int width, int height, int length) override;
    virtual void readPixel(uint8_t **data,int *width,int *height) override;
private:
    void initPbo();
    void initPboArray();
    GLint positionHandle{-1};
    GLint textureHandle{-1};
    GLuint vbo{0};
    GLuint vao{0};
    GLuint ebo{0};
    // 本身图像纹理id
    GLuint imageTextureId{0};
    GLint textureSampler{-1};
    int imageWidth{0};
    int imageHeight{0};
    // 上传纹理的pbo
    GLuint uploadPboId{0};
    // cpu下载纹理的pbo
    GLuint downloadPboId{0};

    // 上传纹理的pbo
    GLuint *uploadPboIds{nullptr};
    // cpu下载纹理的pbo
    GLuint *downloadPboIds{nullptr};

    // Pbo 的索引，用于双PBO时
    int uploadPboIndex{0};
    int downloadPboIndex{0};

};


#endif //NDK_OPENGLES_LEARN_PBOOPENGL_H
