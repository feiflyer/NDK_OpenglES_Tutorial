//
// Created by Fly on 2022/10/25.
//

/**
 * 本教程首发于公众号"思想觉悟"
 * https://mp.weixin.qq.com/s/nHo3yXWdxwijvyLYXvTa7Q
 *
 *
 */
#include "PBOOpengl.h"

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
        1.0f, 1.0f,
        0.5f, 0.5f, // 右上
        // 纹理坐标
        1.0f, 0.0f,
        -0.5f, -0.5f, // 左下
        // 纹理坐标
        0.0f, 1.0f,
        -0.5f, 0.5f, // 左上
        // 纹理坐标
        0.0f, 0.0f
};

// 真正的纹理坐标在图片的左下角
const static GLfloat FBO_VERTICES_AND_TEXTURE[] = {
        1.0f, -1.0f, // 右下
        // 纹理坐标
        1.0f, 0.0f,
        1.0f, 1.0f, // 右上
        // 纹理坐标
        1.0f, 1.0f,
        -1.0f, -1.0f, // 左下
        // 纹理坐标
        0.0f, 0.0f,
        -1.0f, 1.0f, // 左上
        // 纹理坐标
        0.0f, 1.0f
};

// 使用byte类型比使用short或者int类型节约内存
const static uint8_t indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形
        0, 1, 2, // 第一个三角形
        1, 2, 3  // 第二个三角形
};

PBOOpengl::PBOOpengl():uploadPboIds(nullptr),downloadPboIds(nullptr) {
    initGlProgram(ver, fragment);
    positionHandle = glGetAttribLocation(program, "aPosition");
    textureHandle = glGetAttribLocation(program, "aTexCoord");
    textureSampler = glGetUniformLocation(program, "ourTexture");
    LOGD("program:%d", program);
    LOGD("positionHandle:%d", positionHandle);
    LOGD("textureHandle:%d", textureHandle);
    LOGD("textureSample:%d", textureSampler);
    // VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_AND_TEXTURE), VERTICES_AND_TEXTURE,
                 GL_STATIC_DRAW);

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
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 这个顺序不能乱啊，先解除vao，再解除其他的，不然在绘制的时候可能会不起作用，需要重新glBindBuffer才生效
    // vao解除
    glBindVertexArray(0);
    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 解除绑定
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    LOGD("program:%d", program);
    LOGD("positionHandle:%d", positionHandle);
    LOGD("colorHandle:%d", textureHandle);
}

void PBOOpengl::initPbo() {

    int imgByteSize = imageWidth * imageHeight * 4; // RGBA

    glGenBuffers(1, &uploadPboId);
    // 绑定pbo
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPboId);
    // 设置pbo内存大小
    // 这一步十分重要，第2个参数指定了这个缓冲区的大小，单位是字节，一定要注意
    //  然后第3个参数是初始化用的数据，如果你传个内存指针进去，这个函数就会把你的
    //  数据复制到缓冲区里，我们这里一开始并不需要什么数据，所以传个nullptr就行了
    glBufferData(GL_PIXEL_UNPACK_BUFFER, imgByteSize, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glGenBuffers(1, &downloadPboId);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, downloadPboId);
    glBufferData(GL_PIXEL_PACK_BUFFER, imgByteSize, nullptr, GL_STREAM_DRAW);
    // 解除绑定
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    LOGD("uploadPboId:%d---downloadPboId:%d---imgByteSize：%d", uploadPboId, downloadPboId,
         imgByteSize);
}

void PBOOpengl::initPboArray() {
    int imgByteSize = imageWidth * imageHeight * 4; // RGBA
    if(nullptr == uploadPboIds){
        uploadPboIds = new GLuint[NUM_PBO];
        glGenBuffers(NUM_PBO, uploadPboIds);
        for (int i = 0; i < NUM_PBO; ++i) {
            // 绑定pbo
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *(uploadPboIds + i));
            // 设置pbo内存大小
            // 这一步十分重要，第2个参数指定了这个缓冲区的大小，单位是字节，一定要注意
            //  然后第3个参数是初始化用的数据，如果你传个内存指针进去，这个函数就会把你的
            //  数据复制到缓冲区里，我们这里一开始并不需要什么数据，所以传个nullptr就行了
            glBufferData(GL_PIXEL_UNPACK_BUFFER, imgByteSize, nullptr, GL_STREAM_DRAW);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }
    }

    if(nullptr == downloadPboIds){
        downloadPboIds = new GLuint[NUM_PBO];
        glGenBuffers(NUM_PBO, downloadPboIds);
        for (int i = 0; i < NUM_PBO; ++i) {
            // 绑定pbo
            glBindBuffer(GL_PIXEL_PACK_BUFFER, *(downloadPboIds + i));
            glBufferData(GL_PIXEL_PACK_BUFFER, imgByteSize, nullptr, GL_STREAM_DRAW);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        }
    }

}

PBOOpengl::~PBOOpengl() {
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    if(nullptr != uploadPboIds){
        glDeleteBuffers(NUM_PBO,uploadPboIds);
        delete [] uploadPboIds;
        uploadPboIds = nullptr;
    }

    if(nullptr != downloadPboIds){
        glDeleteBuffers(NUM_PBO,downloadPboIds);
        delete [] downloadPboIds;
        downloadPboIds = nullptr;
    }
}

void PBOOpengl::onDraw() {

    // 绘制屏幕宽高
    glViewport(0, 0, eglHelper->viewWidth, eglHelper->viewHeight);

    // 绘制到屏幕
    // 清屏
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 激活纹理
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(textureSampler, 2);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, imageTextureId);

    // VBO与VAO配合绘制
    // 使用vao
    glBindVertexArray(vao);
    // 使用EBO
// 使用byte类型节省内存
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *) 0);
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
//
//// 单个PBO测试
//void PBOOpengl::setPixel(void *data, int width, int height, int length) {
//    LOGD("texture setPixel");
//    imageWidth = width;
//    imageHeight = height;
//    // Pbo初始化
//    initPbo();
//
//    glGenTextures(1, &imageTextureId);
//
//    // 激活纹理，注意以下这个两句是搭配的，glActiveTexture激活的是那个纹理，就设置的sampler2D是那个
//    // 默认是0，如果不是0的话，需要在onDraw的时候重新激活一下？
////    glActiveTexture(GL_TEXTURE0);
////    glUniform1i(textureSampler, 0);
//
//// 例如，一样的
//    glActiveTexture(GL_TEXTURE2);
//    glUniform1i(textureSampler, 2);
//
//    // 绑定纹理
//    glBindTexture(GL_TEXTURE_2D, imageTextureId);
//    // 为当前绑定的纹理对象设置环绕、过滤方式
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // pixels参数传递空，后面会通过pbo更新纹理数据
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//
//    // 生成mip贴图
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    int dataSize = width * height * 4;
//    // 使用Pbo
//    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, uploadPboId);
//    // 将纹理数据拷贝进入缓冲区
//    GLubyte *bufPtr = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0,
//                                                   dataSize,
//                                                   GL_MAP_WRITE_BIT);
//    if (bufPtr) {
//        memcpy(bufPtr, data, static_cast<size_t>(dataSize));
//        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
//    }
//    // 将pbo缓冲区中的数据拷贝到纹理，调用 glTexSubImage2D 后立即返回，不影响 CPU 时钟周期
//    // 这个函数会判断 GL_PIXEL_UNPACK_BUFFER 这个地方有没有绑定一个缓冲区
//    //   如果有，就从这个缓冲区读取数据，而不是data参数指定的那个内存
//    // 这样glTexSubImage2D就会从我们的缓冲区中读取数据了
//    // 这里为什么要用glTexSubImage2D呢，因为如果用glTexImage2D，glTexImage2D会销毁纹理内存重新申请，glTexSubImage2D就仅仅只是更新纹理中的数据，这就提高了速度，并且优化了显存的利用率
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//    // Pbo解除
//    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
//    // 解绑定
//    glBindTexture(GL_TEXTURE_2D, 0);
//}



// 双PBO测试
void PBOOpengl::setPixel(void *data, int width, int height, int length) {
    LOGD("texture setPixel  uploadPboIndex:%d",uploadPboIndex);
    imageWidth = width;
    imageHeight = height;
    // Pbo初始化
    initPboArray();

    if(imageTextureId == 0){

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
        // pixels参数传递空，后面会通过pbo更新纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // 生成mip贴图
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, imageTextureId);

    int dataSize = width * height * 4;
    // 使用Pbo
    // 指针运算你要会一点呀
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *(uploadPboIds + uploadPboIndex % NUM_PBO ));
    // 将纹理数据拷贝进入缓冲区
    GLubyte *bufPtr = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0,
                                                   dataSize,
                                                   GL_MAP_WRITE_BIT);
    if (bufPtr) {
        memcpy(bufPtr, data, static_cast<size_t>(dataSize));
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    // 将pbo缓冲区中的数据拷贝到纹理，调用 glTexSubImage2D 后立即返回，不影响 CPU 时钟周期
    // 这个函数会判断 GL_PIXEL_UNPACK_BUFFER 这个地方有没有绑定一个缓冲区
    //   如果有，就从这个缓冲区读取数据，而不是data参数指定的那个内存
    // 这样glTexSubImage2D就会从我们的缓冲区中读取数据了
    // 这里为什么要用glTexSubImage2D呢，因为如果用glTexImage2D，glTexImage2D会销毁纹理内存重新申请，glTexSubImage2D就仅仅只是更新纹理中的数据，这就提高了速度，并且优化了显存的利用率
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // Pbo解除
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    // 解绑定
    glBindTexture(GL_TEXTURE_2D, 0);
    // 索引自加
    uploadPboIndex++;
}

//// 单PBO读取测试
//void PBOOpengl::readPixel(uint8_t **data,int *width,int *height) {
//
//    *width = eglHelper->viewWidth;
//    *height = eglHelper->viewHeight;
//    int dataSize = eglHelper->viewWidth * eglHelper->viewHeight * 4;
//    *data = new uint8_t[dataSize];
//    // 方法一 正常的glReadPixels读取 最简单
////    glReadPixels(0, 0, eglHelper->viewWidth, eglHelper->viewHeight,
////                 GL_RGBA, GL_UNSIGNED_BYTE, *data);
//
//    // 方法二 pbo读取
//    // 首先我们要把缓冲区绑定到 GL_PIXEL_PACK_BUFFER 这个地方
//    glBindBuffer(GL_PIXEL_PACK_BUFFER, downloadPboId);
//    // 重新分配一下空间
//    glBufferData(GL_PIXEL_PACK_BUFFER, dataSize, nullptr, GL_STREAM_DRAW);
//    // 这个函数会判断 GL_PIXEL_PACK_BUFFER 这个地方有没有绑定一个缓冲区，如果有，那就把数据写入到这个缓冲区里
//    // 前4个参数就是要读取的屏幕区域，不多解释
//    //  格式是RGB，类型是BYTE，每个像素1字节
//    // 如果GL_PIXEL_PACK_BUFFER有绑定缓冲区，最后一个参数就作为偏移值来使用，传nullptr就行
//    glReadPixels(0, 0, *width, *height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//    // 好了我们已经成功把屏幕的像素数据复制到了缓冲区里
//
//    // 这时候，你可以用 glMapBuffer 得到缓冲区的内存指针，来读取里面的像素数据，保存到图片文件
//    // 注意glMapBuffer的第1个参数不一定要是GL_PIXEL_PACK_BUFFER，你可以把缓冲区绑定到比如上面init函数的GL_ARRAY_BUFFER
//    //  然后这里也传GL_ARRAY_BUFFER，由于懒得再绑定一次，就接着用上面绑定的GL_PIXEL_PACK_BUFFER吧
//    GLubyte *mapPtr = static_cast<GLubyte *>(glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, dataSize,
//                                                              GL_MAP_READ_BIT));
//    if (nullptr != mapPtr)
//    {
//        LOGD("readPixel 数据拷贝");
//        // 拷贝数据
//        memcpy(*data,mapPtr,dataSize);
//        // 解除Map
//        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
//    } else{
//        LOGD("readPixel glMapBufferRange null");
//    }
//    // 完事了把GL_PIXEL_PACK_BUFFER这个地方的缓冲区解绑掉，以免别的函数误操作
//    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
//}

// 双PBO读取测试
void PBOOpengl::readPixel(uint8_t **data,int *width,int *height) {

    *width = eglHelper->viewWidth;
    *height = eglHelper->viewHeight;
    int dataSize = eglHelper->viewWidth * eglHelper->viewHeight * 4;
    *data = new uint8_t[dataSize];

    // 首先我们要把缓冲区绑定到 GL_PIXEL_PACK_BUFFER 这个地方
    // 指针运算你要会一点呀
    glBindBuffer(GL_PIXEL_PACK_BUFFER, *(downloadPboIds + downloadPboIndex % NUM_PBO));
    // 重新分配一下空间
    glBufferData(GL_PIXEL_PACK_BUFFER, dataSize, nullptr, GL_STREAM_DRAW);
    // 这个函数会判断 GL_PIXEL_PACK_BUFFER 这个地方有没有绑定一个缓冲区，如果有，那就把数据写入到这个缓冲区里
    // 前4个参数就是要读取的屏幕区域，不多解释
    //  格式是RGB，类型是BYTE，每个像素1字节
    // 如果GL_PIXEL_PACK_BUFFER有绑定缓冲区，最后一个参数就作为偏移值来使用，传nullptr就行
    glReadPixels(0, 0, *width, *height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // 好了我们已经成功把屏幕的像素数据复制到了缓冲区里

    // 这时候，你可以用 glMapBuffer 得到缓冲区的内存指针，来读取里面的像素数据，保存到图片文件
    // 注意glMapBuffer的第1个参数不一定要是GL_PIXEL_PACK_BUFFER，你可以把缓冲区绑定到比如上面init函数的GL_ARRAY_BUFFER
    //  然后这里也传GL_ARRAY_BUFFER，由于懒得再绑定一次，就接着用上面绑定的GL_PIXEL_PACK_BUFFER吧
    GLubyte *mapPtr = static_cast<GLubyte *>(glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, dataSize,
                                                              GL_MAP_READ_BIT));
    if (nullptr != mapPtr)
    {
        LOGD("readPixel 数据拷贝");
        // 拷贝数据
        memcpy(*data,mapPtr,dataSize);
        // 解除Map
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    } else{
        LOGD("readPixel glMapBufferRange null");
    }
    // 完事了把GL_PIXEL_PACK_BUFFER这个地方的缓冲区解绑掉，以免别的函数误操作
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    // Pbo索引自加
    downloadPboIndex++;
}

