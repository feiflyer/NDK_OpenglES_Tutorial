//
// Created by Fly on 2022/9/26.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/aEcr09LBaALcJlDnAmaCuw
 *
 *
 */
#include "FBOOpengl.h"
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

const static GLfloat VERTICES_AND_TEXTURE[] = {
        0.5f, -0.5f, // 右下
        // 纹理坐标
        1.0f,1.0f,
        0.5f, 0.5f, // 右上
        // 纹理坐标
        1.0f,0.0f,
        -0.5f, -0.5f, // 左下
        // 纹理坐标
        0.0f,1.0f,
        -0.5f, 0.5f, // 左上
        // 纹理坐标
        0.0f,0.0f
};

// 真正的纹理坐标在图片的左下角
const static GLfloat FBO_VERTICES_AND_TEXTURE[] = {
        1.0f, -1.0f, // 右下
        // 纹理坐标
        1.0f,0.0f,
        1.0f, 1.0f, // 右上
        // 纹理坐标
        1.0f,1.0f,
        -1.0f, -1.0f, // 左下
        // 纹理坐标
        0.0f,0.0f,
        -1.0f, 1.0f, // 左上
        // 纹理坐标
        0.0f,1.0f
};

// 使用byte类型比使用short或者int类型节约内存
const static uint8_t indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形
        0, 1, 2, // 第一个三角形
        1, 2, 3  // 第二个三角形
};

FBOOpengl::FBOOpengl() {
    initGlProgram(ver,fragment);
    positionHandle = glGetAttribLocation(program,"aPosition");
    textureHandle = glGetAttribLocation(program,"aTexCoord");
    textureSampler = glGetUniformLocation(program,"ourTexture");
    LOGD("program:%d",program);
    LOGD("positionHandle:%d",positionHandle);
    LOGD("textureHandle:%d",textureHandle);
    LOGD("textureSample:%d",textureSampler);
    // VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_AND_TEXTURE), VERTICES_AND_TEXTURE, GL_STATIC_DRAW);

    // stride 步长 每个顶点坐标之间相隔4个数据点，数据类型是float
    glVertexAttribPointer(positionHandle, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    // 启用顶点数据
    glEnableVertexAttribArray(positionHandle);
    // stride 步长 每个颜色坐标之间相隔4个数据点，数据类型是float，颜色坐标索引从2开始
    glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *) (2 * sizeof(float)));
    // 启用纹理坐标数组
    glEnableVertexAttribArray(textureHandle);

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
    LOGD("colorHandle:%d", textureHandle);
}

void FBOOpengl::setPixel(void *data, int width, int height, int length) {
    LOGD("texture setPixel");
    imageWidth = width;
    imageHeight = height;
    glGenTextures(1, &imageTextureId);

    // 激活纹理，注意以下这个两句是搭配的，glActiveTexture激活的是那个纹理，就设置的sampler2D是那个
    // 默认是0，如果不是0的话，需要在onDraw的时候重新激活一下？
//    glActiveTexture(GL_TEXTURE0);
//    glUniform1i(textureSampler, 0);

// 例如，一样的
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(textureSampler, 2);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, imageTextureId);
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

void FBOOpengl::fboPrepare(){

    // VAO
    glGenVertexArrays(1, &fboVao);
    glBindVertexArray(fboVao);

    // vbo
    glGenBuffers(1, &fboVbo);
    glBindBuffer(GL_ARRAY_BUFFER, fboVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FBO_VERTICES_AND_TEXTURE), FBO_VERTICES_AND_TEXTURE, GL_STATIC_DRAW);

    // stride 步长 每个顶点坐标之间相隔4个数据点，数据类型是float
    glVertexAttribPointer(positionHandle, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    // 启用顶点数据
    glEnableVertexAttribArray(positionHandle);
    // stride 步长 每个颜色坐标之间相隔4个数据点，数据类型是float，颜色坐标索引从2开始
    glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *) (2 * sizeof(float)));
    // 启用纹理坐标数组
    glEnableVertexAttribArray(textureHandle);

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

    glGenTextures(1, &fboTextureId);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, fboTextureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenFramebuffers(1,&fboId);
    glBindFramebuffer(GL_FRAMEBUFFER,fboId);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D,fboTextureId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTextureId, 0);
    // 这个纹理是多大的？
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // 检查FBO状态
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE) {
        LOGE("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
    }
    // 解绑
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FBOOpengl::onFboDraw() {
    fboPrepare();

    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    // 主要这个的大小要与FBO绑定时的纹理的glTexImage2D 设置的大小一致呀
    glViewport(0,0,imageWidth,imageHeight);

     // FBO绘制
    // 清屏
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 激活纹理
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(textureSampler, 1);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, imageTextureId);

    // VBO与VAO配合绘制
    // 使用vao
    glBindVertexArray(fboVao);
    // 使用EBO
// 使用byte类型节省内存
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_BYTE,(void *)0);
    glUseProgram(0);
    // vao解除绑定
    glBindVertexArray(0);

    if (nullptr != eglHelper) {
        eglHelper->swapBuffers();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBOOpengl::onDraw() {

    // 先在FBO离屏渲染
    onFboDraw();

    // 恢复绘制屏幕宽高
    glViewport(0,0,eglHelper->viewWidth,eglHelper->viewHeight);

    // 绘制到屏幕
    // 清屏
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 激活纹理
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(textureSampler, 2);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, fboTextureId);

    // VBO与VAO配合绘制
    // 使用vao
    glBindVertexArray(vao);
    // 使用EBO
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

    glBindTexture(GL_TEXTURE_2D, 0);
}


FBOOpengl::~FBOOpengl() noexcept {
    glDeleteBuffers(1,&ebo);
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
    // ... 删除其他，例如fbo等
}