//
// Created by Fly on 2022/9/6.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/x2u6F1weIeOpPDPq48GbRA
 *
 */
#include "RectOpengl.h"
#include "../utils/Log.h"


// 顶点着色器
static const char *ver = "#version 300 es\n"
                         "in vec4 aColor;\n"
                         "in vec4 aPosition;\n"
                         "out vec4 vColor;\n"
                         "void main() {\n"
                         "    vColor = aColor;\n"
                         "    gl_Position = aPosition;\n"
                         "}";

// 片元着色器
static const char *fragment = "#version 300 es\n"
                              "precision mediump float;\n"
                              "in vec4 vColor;\n"
                              "out vec4 fragColor;\n"
                              "void main() {\n"
                              "    fragColor = vColor;\n"
                              "}";

// 使用绘制两个三角形组成一个矩形的形式  GL_TRIANGLES
//const static GLfloat VERTICES[] = {
//        // 第一个三角形
//        0.5f,0.5f,
//        -0.5f,0.5f,
//        -0.5f,-0.5f,
//        // 第二个三角形
//        -0.5f,-0.5f,
//        0.5f,-0.5f,
//        0.5f,0.5f
//};

// 使用绘制两个三角形组成一个矩形的形式(三角形带)
// 第一第二第三个点组成一个三角形，第二第三第四个点组成一个三角形
const static GLfloat VERTICES[] = {
        0.5f,-0.5f, // 右下
        0.5f,0.5f, // 右上
        -0.5f,-0.5f, // 左下
        -0.5f,0.5f // 左上
};


// rgba
const static GLfloat COLOR_ICES[] = {
        0.0f,0.0f,1.0f,1.0f
};

RectOpengl::RectOpengl():BaseOpengl() {
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    colorHandle = glGetAttribLocation(program,"aColor");
    LOGD("program:%d",program);
    LOGD("positionHandle:%d",positionHandle);
    LOGD("colorHandle:%d",colorHandle);
}

void RectOpengl::onDraw() {
    // 清屏
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    /**
     * size 几个数字表示一个点，显示是两个数字表示一个点
     * normalized 是否需要归一化，不用，这里已经归一化了
     * stride 步长，连续顶点之间的间隔，如果顶点直接是连续的，也可填0
     */
    glVertexAttribPointer(positionHandle,2,GL_FLOAT,GL_FALSE,0,VERTICES);
    // 启用顶点数据
    glEnableVertexAttribArray(positionHandle);

    // 这个不需要glEnableVertexAttribArray
    glVertexAttrib4fv(colorHandle, COLOR_ICES);

    // 6个顶点绘制两个三角形组成矩形
    // glDrawArrays(GL_TRIANGLES,0,6);

    // 4个顶点绘制两个三角形组成矩形
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glUseProgram(0);

    // 禁用顶点
    glDisableVertexAttribArray(positionHandle);
    if(nullptr != eglHelper){
        eglHelper->swapBuffers();
    }
}

RectOpengl::~RectOpengl() noexcept {

}
