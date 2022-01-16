//
// Created by 王浩飞 on 2021/12/20.
//

#include <jni.h>

#ifndef OPENGLDEMO_PART3NATIVERENDER_H
#define OPENGLDEMO_PART3NATIVERENDER_H

class Part3NativeRender {
public:
    static Part3NativeRender *GetInstance();

    void OnSurfaceCreate(JNIEnv *env, jobject assetManager);

    void VBO();

    void VAO();

    void OnSurfaceChange(int width, int height);

    void OnDrawFrame();

    int program;

    unsigned int vbo;

    unsigned int vao;

private:
    Part3NativeRender();

    ~Part3NativeRender();

    static Part3NativeRender *instance;
};

#endif //OPENGLDEMO_PART3NATIVERENDER_H
