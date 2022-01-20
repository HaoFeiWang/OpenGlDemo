//
// Created by 王浩飞 on 2021/12/20.
//

#include <jni.h>

#ifndef OPENGLDEMO_PART3NATIVERENDER_H
#define OPENGLDEMO_PART3NATIVERENDER_H

class Part6NativeRender {
public:
    static Part6NativeRender *GetInstance();

    void OnSurfaceCreate(JNIEnv *env, jobject assetManager);

    void OnSurfaceChange(int width, int height);

    void OnDrawFrame();

    void OnTranslation(float x,float y,float z);

    int program;

    unsigned int vbo;

    unsigned int vao;

    unsigned int ebo;

    unsigned int textureId1;
    unsigned int textureId2;

private:
    Part6NativeRender();

    ~Part6NativeRender();

    static Part6NativeRender *instance;

    void createVAO();

    void createVBO();

    void createEBO();

    void createTexture(JNIEnv *env, jobject assetManager);
};

#endif //OPENGLDEMO_PART3NATIVERENDER_H
