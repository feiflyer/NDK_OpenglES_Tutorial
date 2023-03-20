//
// Created by Fly on 2022/12/14.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/QB7jBW8dbtrbZYIgfk8eqQ
 *
 */
#include "../utils/Log.h"
#include "RGBtoYUVOpengl.h"

// 顶点着色器
static const char *ver = "#version 300 es\n"
                         "in vec4 aPosition;\n"
                         "in vec2 aTexCoord;\n"
                         "out vec2 v_texCoord;\n"
                         "void main() {\n"
                         "  v_texCoord = aTexCoord;\n"
                         "  gl_Position = aPosition;\n"
                         "}";

// 片元着色器
static const char *fragment = "#version 300 es\n"
                              "precision mediump float;\n"
                              "in vec2 v_texCoord;\n"
                              "layout(location = 0) out vec4 outColor;\n"
                              "uniform sampler2D s_TextureMap;\n"
                              "uniform float u_Offset;\n"
                              "const vec3 COEF_Y = vec3(0.299, 0.587, 0.114);\n"
                              "const vec3 COEF_U = vec3(-0.147, -0.289, 0.436);\n"
                              "const vec3 COEF_V = vec3(0.615, -0.515, -0.100);\n"
                              "const float UV_DIVIDE_LINE = 2.0 / 3.0;\n"
                              "void main(){\n"
                              "    vec2 texelOffset = vec2(u_Offset, 0.0);\n"
                              "    if (v_texCoord.   y <= UV_DIVIDE_LINE) {\n"
                              "        vec2 texCoord = vec2(v_texCoord.   x, v_texCoord.   y * 3.0 / 2.0);\n"
                              "        vec4 color0 = texture(s_TextureMap, texCoord);\n"
                              "        vec4 color1 = texture(s_TextureMap, texCoord + texelOffset);\n"
                              "        vec4 color2 = texture(s_TextureMap, texCoord + texelOffset * 2.0);\n"
                              "        vec4 color3 = texture(s_TextureMap, texCoord + texelOffset * 3.0);\n"
                              "        float y0 = dot(color0.   rgb, COEF_Y);\n"
                              "        float y1 = dot(color1.   rgb, COEF_Y);\n"
                              "        float y2 = dot(color2.   rgb, COEF_Y);\n"
                              "        float y3 = dot(color3.   rgb, COEF_Y);\n"
                              "        outColor = vec4(y0, y1, y2, y3);\n"
                              "    } else {\n"
                              "        vec2 texCoord = vec2(v_texCoord.x, (v_texCoord.y - UV_DIVIDE_LINE) * 3.0);\n"
                              "        vec4 color0 = texture(s_TextureMap, texCoord);\n"
                              "        vec4 color1 = texture(s_TextureMap, texCoord + texelOffset);\n"
                              "        vec4 color2 = texture(s_TextureMap, texCoord + texelOffset * 2.0);\n"
                              "        vec4 color3 = texture(s_TextureMap, texCoord + texelOffset * 3.0);\n"
                              "        float v0 = dot(color0.   rgb, COEF_V) + 0.5;\n"
                              "        float u0 = dot(color1.   rgb, COEF_U) + 0.5;\n"
                              "        float v1 = dot(color2.   rgb, COEF_V) + 0.5;\n"
                              "        float u1 = dot(color3.   rgb, COEF_U) + 0.5;\n"
                              "        outColor = vec4(v0, u0, v1, u1);\n"
                              "    }\n"
                              "}";


// 使用绘制两个三角形组成一个矩形的形式(三角形带)
// 第一第二第三个点组成一个三角形，第二第三第四个点组成一个三角形
const static GLfloat VERTICES[] = {
        1.0f,-1.0f, // 右下
        1.0f,1.0f, // 右上
        -1.0f,-1.0f, // 左下
        -1.0f,1.0f // 左上
};

// FBO贴图纹理坐标（参考手机屏幕坐标系统，原点在左下角）
// 注意坐标不要错乱
const static GLfloat TEXTURE_COORD[] = {
        1.0f,0.0f, // 右下
        1.0f,1.0f, // 右上
        0.0f,0.0f, // 左下
        0.0f,1.0f // 左上
};

RGBtoYUVOpengl::RGBtoYUVOpengl() {
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    textureHandle = glGetAttribLocation(program,"aTexCoord");
    textureSampler = glGetUniformLocation(program,"s_TextureMap");
    u_Offset = glGetUniformLocation(program,"u_Offset");
    LOGD("program:%d",program);
    LOGD("positionHandle:%d",positionHandle);
    LOGD("textureHandle:%d",textureHandle);
    LOGD("textureSample:%d",textureSampler);
    LOGD("u_Offset:%d",u_Offset);
}

RGBtoYUVOpengl::~RGBtoYUVOpengl() noexcept {

}

void RGBtoYUVOpengl::fboPrepare() {
    glGenTextures(1, &fboTextureId);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, fboTextureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenFramebuffers(1,&fboId);
    glBindFramebuffer(GL_FRAMEBUFFER,fboId);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D,fboTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTextureId, 0);
    // 这个纹理是多大的？
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth / 4, imageHeight * 1.5, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // 检查FBO状态
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE) {
        LOGE("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
    }
    // 解绑
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void RGBtoYUVOpengl::onDraw() {

    // 绘制到FBO上去
    // 绑定fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    // 设置视口大小
    glViewport(0, 0,imageWidth / 4, imageHeight * 1.5);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 激活纹理
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(textureSampler, 2);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, textureId);

    // 设置偏移
    float texelOffset = (float) (1.f / (float) imageWidth);
    glUniform1f(u_Offset,texelOffset);

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

    // 解绑fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RGBtoYUVOpengl::setPixel(void *data, int width, int height, int length) {
    LOGD("texture setPixel");
    imageWidth = width;
    imageHeight = height;

    // 准备fbo
    fboPrepare();

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
    // 解绑定
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RGBtoYUVOpengl::readYUV(uint8_t **data, int *width, int *height) {
    // 从fbo中读取
    // 绑定fbo
    *width = imageWidth;
    *height = imageHeight;
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glBindTexture(GL_TEXTURE_2D, fboTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, fboTextureId, 0);
    *data = new uint8_t[imageWidth * imageHeight * 3 / 2];
    glReadPixels(0, 0, imageWidth / 4, imageHeight * 1.5, GL_RGBA, GL_UNSIGNED_BYTE, *data);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 解绑fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

