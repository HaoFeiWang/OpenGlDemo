//
// Created by WHF on 2022/1/8.
//

#include "log.h"
#include "gl_utils.h"

GLuint compileShader(GLenum shaderType, const char *shaderSource) {
    if (shaderSource == nullptr) {
        LOGE("read shader source error");
        return 0;
    }

    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        LOGE("create shader error");
        return 0;
    }

    //参数2：着色器源码个数
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOGE("compile error = %s", infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint linkShaderProgram(const char *vertexShaderSource,
                         const char *fragmentShaderSource) {
    GLuint program = glCreateProgram();
    if (program == 0) {
        LOGE("create program error");
        return 0;
    }

    GLuint verticalShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(program, verticalShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LOGE("compile error = %s", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}
