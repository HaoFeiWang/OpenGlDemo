//
// Created by 王浩飞 on 2021/12/20.
//

#ifndef OPENGLDEMO_TWONATIVERENDER_H
#define OPENGLDEMO_TWONATIVERENDER_H

class TwoNativeRender {
public:
    static TwoNativeRender *GetInstance();

    void OnSurfaceCreate();

    void OnSurfaceChange(int width, int height);

    void OnDrawFrame();

private:
    TwoNativeRender();

    ~TwoNativeRender();

    static TwoNativeRender *instance;
};

#endif //OPENGLDEMO_TWONATIVERENDER_H
