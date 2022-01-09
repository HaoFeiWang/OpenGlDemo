//
// Created by 王浩飞 on 2021/12/18.
//

#include "../utils/log.h"
#include "../utils/gl_utils.h"
#include "../utils/asset_utils.h"
#include "Part2NativeRender.h"

Part2NativeRender *Part2NativeRender::instance = nullptr;

Part2NativeRender::Part2NativeRender() {
    LOGD("Part2NativeRender create");
}

Part2NativeRender::~Part2NativeRender() {
    LOGD("Part2NativeRender release");
}

Part2NativeRender *Part2NativeRender::GetInstance() {
    if (instance == nullptr) {
        instance = new Part2NativeRender();
    }

    return instance;
}

void Part2NativeRender::OnSurfaceCreate(JNIEnv *env, jobject assetManager) {
    LOGD("Part2NativeRender OnSurfaceCreate");

    //编译链接着色器
    char *vertexSource = readAssetFile(env, assetManager, "glsl/one_vertex.glsl");
    char *fragmentSource = readAssetFile(env, assetManager, "glsl/one_fragment.glsl");

    program = linkShaderProgram(vertexSource, fragmentSource);
    //激活program
    glUseProgram(program);
    //启用顶点属性，顶点属性默认是禁用的
    glEnableVertexAttribArray(0);
}

void Part2NativeRender::OnSurfaceChange(int width, int height) {
    LOGD("Part2NativeRender OnSurfaceChange width = %d height = %d", width, height);
    //设置窗口大小
    glViewport(0, 0, width, height);
}

void Part2NativeRender::OnDrawFrame() {
    float vertexArray[] = {0.0f, 0.5f, 0.0f,   // top
                           -0.5f, -0.5f, 0.0f, // bottom left
                           0.5f, -0.5f, 0.0f}; // bottom right

    //绘制,将顶点数据设置给Program
    //参数一：顶点属性位置
    //参数二：每个顶点的个数
    //参数三：为数据类型
    //参数四：为是否否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
    //参数五：步长，连续的顶点属性组之间的间隔（当数值是紧密排列时，可以设置0来让OpenGL决定具体步长是多少）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

