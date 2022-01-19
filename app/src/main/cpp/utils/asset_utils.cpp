//
// Created by WHF on 2022/1/9.
//

#include "log.h"
#include "gl_utils.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdlib>

char *readAssetFile(JNIEnv *env, jobject javaAssetManager, const char *file_name) {
    AAssetManager *assetManager = AAssetManager_fromJava(env, javaAssetManager);
    AAsset *asset = AAssetManager_open(assetManager, file_name, AASSET_MODE_UNKNOWN);
    if (asset == nullptr) {
        LOGE("open asset error");
        return nullptr;
    }

    off_t fileByteLength = AAsset_getLength(asset);
    LOGD("file byte length = %ld", fileByteLength);

    char *buffer = (char *) malloc(fileByteLength);
    int readLength = AAsset_read(asset, buffer, fileByteLength);
    LOGD("file readLength = %d", readLength);

    AAsset_close(asset);

    return buffer;
}

char *readAssetFile(JNIEnv *env, jobject javaAssetManager, const char *file_name, int *bufferSize) {
    AAssetManager *assetManager = AAssetManager_fromJava(env, javaAssetManager);
    AAsset *asset = AAssetManager_open(assetManager, file_name, AASSET_MODE_UNKNOWN);
    if (asset == nullptr) {
        LOGE("open asset error");
        return nullptr;
    }

    off_t fileByteLength = AAsset_getLength(asset);
    LOGD("file byte length = %ld", fileByteLength);

    *bufferSize = fileByteLength;
    char *buffer = (char *) malloc(fileByteLength);
    int readLength = AAsset_read(asset, buffer, fileByteLength);
    LOGD("file readLength = %d", readLength);

    AAsset_close(asset);

    return buffer;
}