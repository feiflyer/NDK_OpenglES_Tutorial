//
// Created by Fly on 2022/8/25.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/vRiKvtrMkt-S1ShHegvW1A
 *
 */

#include "TriangleOpengl.h"
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

// 三角形三个顶点
const static GLfloat VERTICES[] = {
        0.0f,0.5f,
        -0.5f,-0.5f,
        0.5f,-0.5f
};

// rgba
const static GLfloat COLOR_ICES[] = {
        0.0f,0.0f,1.0f,1.0f
};

TriangleOpengl::TriangleOpengl():BaseOpengl(),positionHandle(-1),colorHandle(-1){
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    colorHandle = glGetAttribLocation(program,"aColor");
    LOGD("program:%d",program);
    LOGD("positionHandle:%d",positionHandle);
    LOGD("colorHandle:%d",colorHandle);
}

TriangleOpengl::~TriangleOpengl() noexcept {

}

void TriangleOpengl::onDraw() {
    LOGD("TriangleOpengl onDraw");
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

    glDrawArrays(GL_TRIANGLES,0,3);

    glUseProgram(0);

    // 禁用顶点
    glDisableVertexAttribArray(positionHandle);
    if(nullptr != eglHelper){
        eglHelper->swapBuffers();
    }

    GLenum error = glGetError();
    LOGD("TriangleOpengl onDraw--end:%d",error);
}



