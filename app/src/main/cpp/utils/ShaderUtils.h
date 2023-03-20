//
// Created by Fly on 2022/8/25.
//

#ifndef NDK_OPENGLES_LEARN_SHADERUTILS_H
#define NDK_OPENGLES_LEARN_SHADERUTILS_H

#include "GLES3/gl3.h"
#include "Log.h"

static int loadShaders(int shaderType, const char *code)
{
    // 按照类型，创建着色器
    int shader = glCreateShader(shaderType);
    // 加载着色器代码
    glShaderSource(shader, 1, &code, 0);
    // 编译
    glCompileShader(shader);
    // 检测编译状态
    int result = GL_FALSE;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&result);
    if(result != GL_TRUE){
        GLint infoLen = 0;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLen);
        char error[infoLen + 1];
        // 获取编译错误
        glGetShaderInfoLog(shader, sizeof(error) / sizeof(error[0]),&infoLen,error);
        LOGE("着色器编译失败:%s,%s",error,code);
    }
    return shader;
}

static int createProgram(const char *vertex , const char * fragment)
{
    int vertexShader = loadShaders(GL_VERTEX_SHADER, vertex);
    int fragmentShader = loadShaders(GL_FRAGMENT_SHADER, fragment);
    int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // 获取一下是否有错误
    GLint status = GL_FALSE;
    glGetProgramiv(program,GL_LINK_STATUS,&status);
    if(status != GL_TRUE){
        GLint infoLen = 0;
        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&infoLen);
        char error[infoLen + 1];
        glGetProgramInfoLog(program,sizeof(error) / sizeof(error[0]) ,&infoLen,error);
        LOGE("着色器程序链接失败:",error);
    }
    // 更严谨一点可通过glValidateProgram()验证着色器程序
    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

#endif //NDK_OPENGLES_LEARN_SHADERUTILS_H
