//
// Created by 王浩飞 on 2021/12/20.
//

#include <jni.h>

#ifndef OPENGLDEMO_PART2NATIVERENDER_H
#define OPENGLDEMO_PART2NATIVERENDER_H

class Part2NativeRender {
public:
    static Part2NativeRender *GetInstance();

    void OnSurfaceCreate(JNIEnv *env, jobject assetManager);

    void OnSurfaceChange(int width, int height);

    void OnDrawFrame();

    int program;

private:
    Part2NativeRender();

    ~Part2NativeRender();

    static Part2NativeRender *instance;
};

#endif //OPENGLDEMO_PART2NATIVERENDER_H
