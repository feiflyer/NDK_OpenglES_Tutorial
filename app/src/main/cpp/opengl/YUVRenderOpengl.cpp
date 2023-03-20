//
// Created by Fly on 2022/11/1.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/mrPgqOYYElR2A-fLdZ2JFQ
 *
 */
#include "YUVRenderOpengl.h"

#include "../utils/Log.h"

// 顶点着色器
static const char *ver = "#version 300 es\n"
                         "in vec4 aPosition;\n"
                         "in vec2 aTexCoord;\n"
                         "out vec2 TexCoord;\n"
                         "void main() {\n"
                         "  TexCoord = aTexCoord;\n"
                         "  gl_Position = aPosition;\n"
                         "}";

// 片元着色器 nv12
//static const char *fragment = "#version 300 es\n"
//                              "precision mediump float;\n"
//                              "out vec4 FragColor;\n"
//                              "in vec2 TexCoord;\n"
//                              "uniform sampler2D y_texture; \n"
//                              "uniform sampler2D uv_texture;\n"
//                              "void main()\n"
//                              "{\n"
//                              "vec3 yuv;\n"
//                              "yuv.x = texture(y_texture, TexCoord).r;\n"
//                              "yuv.y = texture(uv_texture, TexCoord).r-0.5;\n"
//                              "yuv.z = texture(uv_texture, TexCoord).a-0.5;\n"
//                              "vec3 rgb =mat3( 1.0,1.0,1.0,\n"
//                              "0.0,-0.344,1.770,1.403,-0.714,0.0) * yuv;\n"
//                              "FragColor = vec4(rgb, 1);\n"
//                              "}";

/**
 *  仅仅是以下两句不同而已
 *  "yuv.y = texture(uv_texture, TexCoord).r-0.5;\n"
 *  "yuv.z = texture(uv_texture, TexCoord).a-0.5;\n"
 */
// 片元着色器nv21 仅仅是
static const char *fragment = "#version 300 es\n"
                              "precision mediump float;\n"
                              "out vec4 FragColor;\n"
                              "in vec2 TexCoord;\n"
                              "uniform sampler2D y_texture; \n"
                              "uniform sampler2D uv_texture;\n"
                              "void main()\n"
                              "{\n"
                              "vec3 yuv;\n"
                              "yuv.x = texture(y_texture, TexCoord).r;\n"
                              "yuv.y = texture(uv_texture, TexCoord).a-0.5;\n"
                              "yuv.z = texture(uv_texture, TexCoord).r-0.5;\n"
                              "vec3 rgb =mat3( 1.0,1.0,1.0,\n"
                              "0.0,-0.344,1.770,1.403,-0.714,0.0) * yuv;\n"
                              "FragColor = vec4(rgb, 1);\n"
                              "}";

// 使用绘制两个三角形组成一个矩形的形式(三角形带)
// 第一第二第三个点组成一个三角形，第二第三第四个点组成一个三角形
const static GLfloat VERTICES[] = {
        0.5f,-0.5f, // 右下
        0.5f,0.5f, // 右上
        -0.5f,-0.5f, // 左下
        -0.5f,0.5f // 左上
};

// 贴图纹理坐标（参考手机屏幕坐标系统，原点在左上角）
//由于对一个OpenGL纹理来说，它没有内在的方向性，因此我们可以使用不同的坐标把它定向到任何我们喜欢的方向上，然而大多数计算机图像都有一个默认的方向，它们通常被规定为y轴向下，X轴向右
const static GLfloat TEXTURE_COORD[] = {
        1.0f,1.0f, // 右下
        1.0f,0.0f, // 右上
        0.0f,1.0f, // 左下
        0.0f,0.0f // 左上
};

YUVRenderOpengl::YUVRenderOpengl() {
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    textureHandle = glGetAttribLocation(program,"aTexCoord");
    y_textureSampler = glGetUniformLocation(program,"y_texture");
    uv_textureSampler = glGetUniformLocation(program,"uv_texture");
    LOGD("program:%d",program);
    LOGD("positionHandle:%d",positionHandle);
    LOGD("textureHandle:%d",textureHandle);
    LOGD("y_textureSampler:%d",y_textureSampler);
    LOGD("uv_textureSampler:%d",uv_textureSampler);
}

YUVRenderOpengl::~YUVRenderOpengl() {

}

void YUVRenderOpengl::setYUVData(void *y_data, void *uv_data, int width, int height, int yuvType) {

    // 准备y数据纹理
    glGenTextures(1, &y_textureId);
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(y_textureSampler, 2);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, y_textureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, y_data);
    // 生成mip贴图
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, y_textureId);
    // 解绑定
    glBindTexture(GL_TEXTURE_2D, 0);

    // 准备uv数据纹理
    glGenTextures(1, &uv_textureId);
    glActiveTexture(GL_TEXTURE3);
    glUniform1i(uv_textureSampler, 3);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, uv_textureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 注意宽高
    // 注意要使用 GL_LUMINANCE_ALPHA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width/2, height/2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, uv_data);
    // 生成mip贴图
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, uv_textureId);
    // 解绑定
    glBindTexture(GL_TEXTURE_2D, 0);
}

void YUVRenderOpengl::onDraw() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 激活纹理
    glActiveTexture(GL_TEXTURE2);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, y_textureId);
    glUniform1i(y_textureSampler, 2);

    // 激活纹理
    glActiveTexture(GL_TEXTURE3);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, uv_textureId);
    glUniform1i(uv_textureSampler, 3);

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
