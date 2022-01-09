//
// Created by WHF on 2022/1/9.
//
#include <jni.h>

#ifndef OPENGLDEMO_ASSET_UTILS_H
#define OPENGLDEMO_ASSET_UTILS_H

char *readAssetFile(JNIEnv *env, jobject assetManager, const char *file_name);

#endif //OPENGLDEMO_ASSET_UTILS_H
