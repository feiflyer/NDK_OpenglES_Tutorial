//
// Created by Fly on 2022/9/19.
//

#include "MatrixTransformOpengl.h"
#include "../utils/Log.h"
#include "glm/glm.hpp"

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/Z5V7W6bwbamCYMfmFvB2LA
 *
 * https://mp.weixin.qq.com/s/e49kHBlhmpDNtOYG46KJgA
 *
 */
// 顶点着色器
static const char *ver = "#version 300 es\n"
                         "in vec4 aPosition;\n"
                         "in vec2 aTexCoord;\n"
                         "out vec2 TexCoord;\n"
                         "uniform mat4 mvpMatrix;\n"
                         "void main() {\n"
                         "  TexCoord = aTexCoord;\n"
                         "  gl_Position = mvpMatrix * aPosition;\n"
                         "}";

// 片元着色器
static const char *fragment = "#version 300 es\n"
                              "precision mediump float;\n"
                              "out vec4 FragColor;\n"
                              "in vec2 TexCoord;\n"
                              "uniform sampler2D ourTexture;\n"
                              "void main()\n"
                              "{\n"
                              "    FragColor = texture(ourTexture, TexCoord);\n"
                              "}";


// 使用绘制两个三角形组成一个矩形的形式(三角形带)
// 第一第二第三个点组成一个三角形，第二第三第四个点组成一个三角形
const static GLfloat VERTICES[] = {
        1.0f,-1.0f, // 右下
        1.0f,1.0f, // 右上
        -1.0f,-1.0f, // 左下
        -1.0f,1.0f // 左上
};

// 贴图纹理坐标（参考手机屏幕坐标系统，原点在左上角）
//由于对一个OpenGL纹理来说，它没有内在的方向性，因此我们可以使用不同的坐标把它定向到任何我们喜欢的方向上，然而大多数计算机图像都有一个默认的方向，它们通常被规定为y轴向下，X轴向右
const static GLfloat TEXTURE_COORD[] = {
        1.0f,1.0f, // 右下
        1.0f,0.0f, // 右上
        0.0f,1.0f, // 左下
        0.0f,0.0f // 左上
};

MatrixTransformOpengl::MatrixTransformOpengl():BaseOpengl() {
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    textureHandle = glGetAttribLocation(program,"aTexCoord");
    textureSampler = glGetUniformLocation(program,"ourTexture");
    matrixHandle = glGetUniformLocation(program,"mvpMatrix");
}

MatrixTransformOpengl::~MatrixTransformOpengl() noexcept {
    LOGD("MatrixTransformOpengl析构函数");
}

void MatrixTransformOpengl::setMvpMatrix(float *mvp) {
    for (int i = 0; i < 16; ++i) {
        mvpMatrix[i] = mvp[i];
    }
}

void MatrixTransformOpengl::setPixel(void *data, int width, int height, int length) {
    LOGD("texture setPixel");
    imageWidth = width;
    imageHeight = height;
    glGenTextures(1, &textureId);

    // 激活纹理，注意以下这个两句是搭配的，glActiveTexture激活的是那个纹理，就设置的sampler2D是那个
    // 默认是0，如果不是0的话，需要在onDraw的时候重新激活一下？
//    glActiveTexture(GL_TEXTURE0);
//    glUniform1i(textureSampler, 0);

// 例如，一样的
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(textureSampler, 2);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, textureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // 生成mip贴图
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textureId);

    // 解绑定
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MatrixTransformOpengl::onDraw() {

//    glViewport(0,0,imageWidth,imageHeight);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 激活纹理
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(textureSampler, 2);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, textureId);

    // 设置矩阵
    glUniformMatrix4fv(matrixHandle, 1, GL_FALSE,mvpMatrix);

    /**
     * size 几个数字表示一个点，显示是两个数字表示一个点
     * normalized 是否需要归一化，不用，这里已经归一化了
     * stride 步长，连续顶点之间的间隔，如果顶点直接是连续的，也可填0
     */
    // 启用顶点数据
    glEnableVertexAttribArray(positionHandle);
    glVertexAttribPointer(positionHandle,2,GL_FLOAT,GL_FALSE,0,VERTICES);

    // 纹理坐标
    glEnableVertexAttribArray(textureHandle);
    glVertexAttribPointer(textureHandle,2,GL_FLOAT,GL_FALSE,0,TEXTURE_COORD);

    // 4个顶点绘制两个三角形组成矩形
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glUseProgram(0);

    // 禁用顶点
    glDisableVertexAttribArray(positionHandle);
    if(nullptr != eglHelper){
        eglHelper->swapBuffers();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

