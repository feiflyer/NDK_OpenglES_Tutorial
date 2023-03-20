//
// Created by Fly on 2022/9/22.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/DTLgelOISImvd7CxJRkS9A
 *
 *
 */
#include "VBOVAOOpengl.h"
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

// 使用绘制两个三角形组成一个矩形的形式(三角形带)
// 第一第二第三个点组成一个三角形，第二第三第四个点组成一个三角形
const static GLfloat VERTICES[] = {
        0.5f,-0.5f, // 右下
        0.5f,0.5f, // 右上
        -0.5f,-0.5f, // 左下
        -0.5f,0.5f, // 左上
};

const static GLfloat VERTICES_AND_COLOR[] = {
        0.5f,-0.5f, // 右下
        // 颜色
        0.0f,0.0f,1.0f,1.0f,
        0.5f,0.5f, // 右上
        // 颜色
        0.0f,0.0f,1.0f,1.0f,
        -0.5f,-0.5f, // 左下
        // 颜色
        0.0f,0.0f,1.0f,1.0f,
        -0.5f,0.5f, // 左上
        // 颜色
        0.0f,0.0f,1.0f,1.0f,
};

// rgba
//const static GLfloat COLOR_ICES[] = {
//        0.0f,0.0f,1.0f,1.0f
//};

// vbo颜色
const static GLfloat COLOR_ICES[] = {
        0.0f,0.0f,1.0f,1.0f,
        0.0f,0.0f,1.0f,1.0f,
        0.0f,0.0f,1.0f,1.0f,
        0.0f,0.0f,1.0f,1.0f,
};

VBOVAOOpengl::VBOVAOOpengl() {
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    colorHandle = glGetAttribLocation(program,"aColor");
    // vbo
    glGenBuffers(3,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(VERTICES),VERTICES,GL_STATIC_DRAW);

    // 颜色
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(COLOR_ICES),COLOR_ICES,GL_STATIC_DRAW);

    // 顶点与颜色混合，先顶点坐标，再颜色坐标
    glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(VERTICES_AND_COLOR),VERTICES_AND_COLOR
                 ,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    // VAO
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    // VAO与VBO关联
    glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
    // stride 步长 每个顶点坐标之间相隔6个数据点，数据类型是float
    glVertexAttribPointer(positionHandle,2,GL_FLOAT,GL_FALSE,6 * sizeof(float ),(void *)0);
    // 启用顶点数据
    glEnableVertexAttribArray(positionHandle);
    // stride 步长 每个颜色坐标之间相隔6个数据点，数据类型是float，颜色坐标索引从2开始
    glVertexAttribPointer(colorHandle,4,GL_FLOAT,GL_FALSE,6 * sizeof(float ) ,(void *)(2 * sizeof(float)) );
    // 启用颜色顶点数据
    glEnableVertexAttribArray(colorHandle);
    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);

    LOGD("program:%d",program);
    LOGD("positionHandle:%d",positionHandle);
    LOGD("colorHandle:%d",colorHandle);
}

void VBOVAOOpengl::onDraw() {
    // 清屏
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 普通方式绘制
//    /**
//     * size 几个数字表示一个点，显示是两个数字表示一个点
//     * normalized 是否需要归一化，不用，这里已经归一化了
//     * stride 步长，连续顶点之间的间隔，如果顶点直接是连续的，也可填0
//     */
//    glVertexAttribPointer(positionHandle,2,GL_FLOAT,GL_FALSE,0,VERTICES);
//    // 启用顶点数据
//    glEnableVertexAttribArray(positionHandle);
//
//    // 这个不需要glEnableVertexAttribArray
//    glVertexAttrib4fv(colorHandle, COLOR_ICES);

// ############################################# 分割线 #################################
     // 使用VBO的方式绘制，顶点与颜色分开
    //    /**
//    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
//    glVertexAttribPointer(positionHandle,2,GL_FLOAT,GL_FALSE,0,(void *)0);
//    // 启用顶点数据
//    glEnableVertexAttribArray(positionHandle);
//
//    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
//    glVertexAttribPointer(colorHandle,4,GL_FLOAT,GL_FALSE,0,(void *)0);
//    // 启用颜色顶点数据
//    glEnableVertexAttribArray(colorHandle);
//    // 解除绑定
//    glBindBuffer(GL_ARRAY_BUFFER,0);

// ############################################# 分割线 #################################
    // VBO绘制 顶点坐标与颜色坐标一起
//    glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
//    // stride 步长 每个顶点坐标之间相隔6个数据点，数据类型是float
//    glVertexAttribPointer(positionHandle,2,GL_FLOAT,GL_FALSE,6 * sizeof(float ),(void *)0);
//    // 启用顶点数据
//    glEnableVertexAttribArray(positionHandle);
//    // stride 步长 每个颜色坐标之间相隔6个数据点，数据类型是float，颜色坐标索引从2开始
//    glVertexAttribPointer(colorHandle,4,GL_FLOAT,GL_FALSE,6 * sizeof(float ) ,(void *)(2 * sizeof(float)) );
//    // 启用颜色顶点数据
//    glEnableVertexAttribArray(colorHandle);
//    // 解除绑定
//    glBindBuffer(GL_ARRAY_BUFFER,0);

// ############################################# 分割线 #################################

    // VBO与VAO配合绘制
    // 使用vao
    glBindVertexArray(vao);
    // 4个顶点绘制两个三角形组成矩形
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glUseProgram(0);
    // vao解除绑定
    glBindVertexArray(vao);

    // 禁用顶点
    glDisableVertexAttribArray(positionHandle);
    if(nullptr != eglHelper){
        eglHelper->swapBuffers();
    }
}

VBOVAOOpengl::~VBOVAOOpengl() noexcept {
    glDeleteBuffers(NUM_VBO,vbo);
    glDeleteVertexArrays(1,&vao);
}
