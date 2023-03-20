//
// Created by Fly on 2022/8/25.
//

#include "BaseOpengl.h"
#include "../utils/ShaderUtils.h"

BaseOpengl::BaseOpengl():program(0) {

}

void BaseOpengl::initGlProgram(std::string ver, std::string fragment) {
    char c_ver[ver.length()];
    char c_frag[fragment.length()];
    strcpy(c_ver,ver.c_str());
    strcpy(c_frag,fragment.c_str());
    program = createProgram(c_ver,c_frag);
}

void BaseOpengl::setPixel(void *data, int width, int height, int length) {

}

void BaseOpengl::setPixel2(void *data, int width, int height, int length) {

}

void BaseOpengl::setLutPixel(void *data, int width, int height, int length) {

}

void BaseOpengl::setWaterPixel(void *data, int width, int height, int length) {

}

void BaseOpengl::readPixel(uint8_t **data,int *width,int *height) {

}

void BaseOpengl::setMvpMatrix(float *mvp) {

}

void BaseOpengl::readYUV(uint8_t **data, int *width, int *height) {
    LOGD("readYUV");
}

void BaseOpengl::setYUVData(void *y_data, void *uv_data, int width, int height, int yuvType) {

}

void BaseOpengl::setLutCubeData(int width, int height, int depth, float *data) {

}

void BaseOpengl::checkError(GLuint program) {
   GLuint error = glGetError();
   if(error != GL_NO_ERROR){
       char errors[1024];
       int  length;
       glGetProgramInfoLog(program,1024,&length,errors);
       LOGD("checkError:%s",errors);
   }
}

BaseOpengl::~BaseOpengl(){
    LOGD("BaseOpengl析构函数");
    if(program != 0){
        glDeleteProgram(program);
    }
    eglHelper = nullptr;
}