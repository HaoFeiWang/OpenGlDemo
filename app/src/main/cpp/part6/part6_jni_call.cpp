//
// Created by 王浩飞 on 2021/12/18.
//

#include <jni.h>
#include "../utils/log.h"
#include "Part6NativeRender.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL native_onSurfaceCreated6(JNIEnv *env, jobject, jobject assetManager) {
    LOGD("native_onSurfaceCreated");
    Part6NativeRender::GetInstance()->OnSurfaceCreate(env, assetManager);
}

JNIEXPORT void JNICALL native_onSurfaceChanged6(JNIEnv *env, jobject, jint width, jint height) {
    LOGD("native_onSurfaceChanged");
    Part6NativeRender::GetInstance()->OnSurfaceChange(width, height);
}

JNIEXPORT void JNICALL native_onDrawFrame6(JNIEnv *env, jobject) {
    //LOGD("native_onDrawFrame");
    Part6NativeRender::GetInstance()->OnDrawFrame();
}

JNIEXPORT void JNICALL native_onTranslation(JNIEnv *env, jobject, jfloat x, jfloat y, jfloat z) {
    LOGD("native_onTranslation x = %f y = %f z = %f", x, y, z);
    Part6NativeRender::GetInstance()->OnTranslation(x, y, z);
}

/**
 * JNI函数注册表
 * translation(int x, int y, int z)
 */
static JNINativeMethod jni_method_table[] = {
        {"onSurfaceCreated", "(Landroid/content/res/AssetManager;)V", (void *) (native_onSurfaceCreated6)},
        {"onSurfaceChanged", "(II)V",                                 (void *) (native_onSurfaceChanged6)},
        {"onDrawFrame",      "()V",                                   (void *) (native_onDrawFrame6)},
        {"translation",      "(FFF)V",                                (void *) (native_onTranslation)}
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("JNI_OnLoad");

    JNIEnv *jniEnv = nullptr;
    if (vm->GetEnv((void **) &jniEnv, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("JNI_OnLoad GetEnv error");
        return JNI_ERR;
    }

    jclass jniCallClass = jniEnv->FindClass("com/whf/opengldemo/part6/Part6JniCall");
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