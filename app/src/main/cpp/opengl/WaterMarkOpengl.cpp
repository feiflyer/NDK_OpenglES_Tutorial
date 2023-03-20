//
// Created by Fly on 2023/2/21.
//

#include "WaterMarkOpengl.h"
#include "../utils/Log.h"


/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/yGRmVMy7mvnj40Zh2WqaeQ
 *
 */

// 顶点着色器
static const char *ver = "#version 300 es\n"
                         "in vec4 aPosition;\n"
                         "in vec2 aTexCoord;\n"
                         "out vec2 TexCoord;\n"
                         "void main() {\n"
                         "  TexCoord = aTexCoord;\n"
                         "  gl_Position = aPosition;\n"
                         "}";

// 片元着色器
// 多纹理重叠，通过mix混合实现水印
//static const char *fragment = "#version 300 es\n"
//                              "precision mediump float;\n"
//                              "out vec4 FragColor;\n"
//                              "in vec2 TexCoord;\n"
//                              "uniform sampler2D ourTexture;\n"
//                              "uniform sampler2D waterTexture;\n"
//                              "void main()\n"
//                              "{\n"
//                              "    vec4 textColor = texture(ourTexture, TexCoord);\n"
//                              "    vec4 waterColor = texture(waterTexture, TexCoord);\n"
//                              "    FragColor = mix(textColor, waterColor, 0.7);\n"
//                              "}";

static const char *fragment = "#version 300 es\n"
                              "precision mediump float;\n"
                              "out vec4 FragColor;\n"
                              "in vec2 TexCoord;\n"
                              "uniform sampler2D ourTexture;\n"
                              "void main()\n"
                              "{\n"
                              "    vec4 textColor = texture(ourTexture, TexCoord);\n"
                              "    FragColor = textColor;\n"
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
const static GLfloat TEXTURE_COORD[] = {
        1.0f,1.0f, // 右下
        1.0f,0.0f, // 右上
        0.0f,1.0f, // 左下
        0.0f,0.0f // 左上
};

WaterMarkOpengl::WaterMarkOpengl() {
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    textureHandle = glGetAttribLocation(program,"aTexCoord");
    textureSampler = glGetUniformLocation(program,"ourTexture");
    waterTextureSampler = glGetUniformLocation(program,"waterTexture");
}

WaterMarkOpengl::~WaterMarkOpengl() noexcept {

}

void WaterMarkOpengl::onDraw() {

//    // 可以注释掉这两句看看效果是不一样的
////    glEnable(GL_BLEND); //打开混合功能
////    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //指定混合模式
//
//    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glUseProgram(program);
//
//    // 激活纹理
//    glActiveTexture(GL_TEXTURE2);
//    // 绑定纹理
//    glBindTexture(GL_TEXTURE_2D, textureId);
//    glUniform1i(textureSampler, 2);
//
//
//    // 激活纹理
//    glActiveTexture(GL_TEXTURE3);
//    // 绑定纹理
//    glBindTexture(GL_TEXTURE_2D, waterTextureId);
//    glUniform1i(waterTextureSampler, 3);
//
//    /**
//     * size 几个数字表示一个点，显示是两个数字表示一个点
//     * normalized 是否需要归一化，不用，这里已经归一化了
//     * stride 步长，连续顶点之间的间隔，如果顶点直接是连续的，也可填0
//     */
//    // 启用顶点数据
//    glEnableVertexAttribArray(positionHandle);
//    glVertexAttribPointer(positionHandle,2,GL_FLOAT,GL_FALSE,0,VERTICES);
//
//    // 纹理坐标
//    glEnableVertexAttribArray(textureHandle);
//    glVertexAttribPointer(textureHandle,2,GL_FLOAT,GL_FALSE,0,TEXTURE_COORD);
//
//    // 4个顶点绘制两个三角形组成矩形
//    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
//
//    glUseProgram(0);
//
//    // 禁用顶点
//    glDisableVertexAttribArray(positionHandle);
//    if(nullptr != eglHelper){
//        eglHelper->swapBuffers();
//    }
//
//    glBindTexture(GL_TEXTURE_2D, 0);

    // 可以注释掉这两句看看效果是不一样的
    glEnable(GL_BLEND); //打开混合功能
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //指定混合模式

    // 先绘制背景，正常纹理
   // 让纹理图片居中
    glViewport((eglHelper->viewWidth - imageWidth)/2,(eglHelper->viewHeight - imageHeight)/2,imageWidth,imageHeight);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 激活纹理
    glActiveTexture(GL_TEXTURE2);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureSampler, 2);

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

    // 然后使用统一的着色器，改变纹理数据再绘制一次水印 顶点坐标和纹理坐标和上面使用的一样，因此不用重新赋值了
    // 调整窗口位置，调整水印的绘制位置，让水印绘制的背景的右下角
    int waterWidth = 200;
    int waterHeight = 300;
    // 以手机屏幕坐下角为坐标原点
    glViewport((eglHelper->viewWidth - imageWidth)/2 + imageWidth - waterWidth,(eglHelper->viewHeight - imageHeight)/2,waterWidth,waterHeight);
    // 激活纹理
    glActiveTexture(GL_TEXTURE3);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, waterTextureId);
    glUniform1i(textureSampler, 3);
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

void WaterMarkOpengl::setPixel(void *data, int width, int height, int length) {
    LOGD("texture setPixel");
    imageWidth = width;
    imageHeight = height;
    glGenTextures(1, &textureId);
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
    // 解绑定
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterMarkOpengl::setWaterPixel(void *data, int width, int height, int length) {
    LOGD("texture setWaterPixel");
    glGenTextures(1, &waterTextureId);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, waterTextureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // 生成mip贴图
    glGenerateMipmap(GL_TEXTURE_2D);

    // 解绑定
    glBindTexture(GL_TEXTURE_2D, 0);
}