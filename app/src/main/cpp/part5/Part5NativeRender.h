//
// Created by 王浩飞 on 2021/12/20.
//

#include <jni.h>

#ifndef OPENGLDEMO_PART3NATIVERENDER_H
#define OPENGLDEMO_PART3NATIVERENDER_H

class Part5NativeRender {
public:
    static Part5NativeRender *GetInstance();

    void OnSurfaceCreate(JNIEnv *env, jobject assetManager);

    void OnSurfaceChange(int width, int height);

    void OnDrawFrame();

    int program;

    unsigned int vbo;

    unsigned int vao;

    unsigned int ebo;

    unsigned int textureId;

private:
    Part5NativeRender();

    ~Part5NativeRender();

    static Part5NativeRender *instance;

    void createVAO();

    void createVBO();

    void createEBO();

    void createTexture(JNIEnv *env, jobject assetManager);
};

#endif //OPENGLDEMO_PART3NATIVERENDER_H
