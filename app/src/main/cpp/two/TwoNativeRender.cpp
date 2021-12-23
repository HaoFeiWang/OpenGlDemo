//
// Created by 王浩飞 on 2021/12/18.
//

#include "../utils/log.h"
#include "TwoNativeRender.h"


TwoNativeRender *TwoNativeRender::instance = nullptr;

TwoNativeRender::TwoNativeRender(){
    LOGD("TwoNativeRender create");
}

TwoNativeRender::~TwoNativeRender(){
    LOGD("TwoNativeRender release");
}

TwoNativeRender *TwoNativeRender::GetInstance() {
    if (instance == nullptr) {
        instance = new TwoNativeRender();
    }

    return instance;
}


void TwoNativeRender::OnSurfaceCreate() {
    LOGD("TwoNativeRender OnSurfaceCreate");
}

void TwoNativeRender::OnSurfaceChange(int width, int height) {
    //LOGD("TwoNativeRender OnSurfaceChange width = %d height = %d", width, height);
}

void TwoNativeRender::OnDrawFrame() {
    LOGD("TwoNativeRender OnDrawFrame");
}

