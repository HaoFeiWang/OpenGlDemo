//
// Created by 王浩飞 on 2021/12/18.
//

#include <jni.h>
#include "../utils/log.h"
#include "Part3NativeRender.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL native_onSurfaceCreated3(JNIEnv *env, jobject, jobject assetManager) {
    LOGD("native_onSurfaceCreated");
    Part3NativeRender::GetInstance()->OnSurfaceCreate(env, assetManager);
}

JNIEXPORT void JNICALL native_onSurfaceChanged3(JNIEnv *env, jobject, jint width, jint height) {
    LOGD("native_onSurfaceChanged");
    Part3NativeRender::GetInstance()->OnSurfaceChange(width, height);
}

JNIEXPORT void JNICALL native_onDrawFrame3(JNIEnv *env, jobject) {
    //LOGD("native_onDrawFrame");
    Part3NativeRender::GetInstance()->OnDrawFrame();
}

/**
 * JNI函数注册表
 */
static JNINativeMethod jni_method_table[] = {
        {"onSurfaceCreated", "(Landroid/content/res/AssetManager;)V",   (void *) (native_onSurfaceCreated3)},
        {"onSurfaceChanged", "(II)V", (void *) (native_onSurfaceChanged3)},
        {"onDrawFrame",      "()V",   (void *) (native_onDrawFrame3)}
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("JNI_OnLoad");

    JNIEnv *jniEnv = nullptr;
    if (vm->GetEnv((void **) &jniEnv, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("JNI_OnLoad GetEnv error");
        return JNI_ERR;
    }

    jclass jniCallClass = jniEnv->FindClass("com/whf/opengldemo/part3/Part3JniCall");
    if (jniCallClass == nullptr) {
        LOGE("JNI_OnLoad FindClass error");
        return JNI_ERR;
    }

    int tableNum = sizeof(jni_method_table) / sizeof(jni_method_table[0]);
    if (jniEnv->RegisterNatives(jniCallClass, jni_method_table, tableNum) != JNI_OK) {
        LOGE("JNI_OnLoad RegisterNatives error");
        return JNI_ERR;
    }

    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif