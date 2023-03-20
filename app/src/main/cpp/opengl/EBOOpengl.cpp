//
// Created by Fly on 2022/9/23.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/KM0GEUYA8cwsf5ePXrO8kA
 *
 *
 */
#include "EBOOpengl.h"

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

const static GLfloat VERTICES_AND_COLOR[] = {
        0.5f, -0.5f, // 右下
        // 颜色
        0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, // 右上
        // 颜色
        0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, // 左下
        // 颜色
        0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, // 左上
        // 颜色
        0.0f, 0.0f, 1.0f, 1.0f,
};

//const static unsigned int indices[] = {
//        // 注意索引从0开始!
//        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
//        // 这样可以由下标代表顶点组合成矩形
//        0, 1, 2, // 第一个三角形
//        1, 2, 3  // 第二个三角形
//};

//// 使用shor类型
//const static unsigned short indices[] = {
//        // 注意索引从0开始!
//        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
//        // 这样可以由下标代表顶点组合成矩形
//        0, 1, 2, // 第一个三角形
//        1, 2, 3  // 第二个三角形
//};

// 使用byte类型
const static uint8_t indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形
        0, 1, 2, // 第一个三角形
        1, 2, 3  // 第二个三角形
};

EBOOpengl::EBOOpengl() {
    initGlProgram(ver, fragment);
    positionHandle = glGetAttribLocation(program, "aPosition");
    colorHandle = glGetAttribLocation(program, "aColor");

    // VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_AND_COLOR), VERTICES_AND_COLOR, GL_STATIC_DRAW);

    // stride 步长 每个顶点坐标之间相隔6个数据点，数据类型是float
    glVertexAttribPointer(positionHandle, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    // 启用顶点数据
    glEnableVertexAttribArray(positionHandle);
    // stride 步长 每个颜色坐标之间相隔6个数据点，数据类型是float，颜色坐标索引从2开始
    glVertexAttribPointer(colorHandle, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *) (2 * sizeof(float)));
    // 启用颜色顶点数据
    glEnableVertexAttribArray(colorHandle);

    // EBO
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    // 这个顺序不能乱啊，先解除vao，再解除其他的，不然在绘制的时候可能会不起作用，需要重新glBindBuffer才生效
    // vao解除
    glBindVertexArray(0);
    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 解除绑定
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    LOGD("program:%d", program);
    LOGD("positionHandle:%d", positionHandle);
    LOGD("colorHandle:%d", colorHandle);
}

void EBOOpengl::onDraw() {
    // 清屏
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // VBO与VAO配合绘制
    // 使用vao
    glBindVertexArray(vao);
    // 使用EBO
//    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void *)0);
// 使用short类型节省内存
//    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,(void *)0);
// 使用byte类型节省内存
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_BYTE,(void *)0);
    glUseProgram(0);
    // vao解除绑定
    glBindVertexArray(0);

    // 禁用顶点
    glDisableVertexAttribArray(positionHandle);
    if (nullptr != eglHelper) {
        eglHelper->swapBuffers();
    }
}

EBOOpengl::~EBOOpengl() noexcept {
    glDeleteBuffers(1,&ebo);
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
}
