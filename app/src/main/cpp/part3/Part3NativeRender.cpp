//
// Created by 王浩飞 on 2021/12/18.
//

#include "../utils/log.h"
#include "../utils/gl_utils.h"
#include "../utils/asset_utils.h"
#include "Part3NativeRender.h"

Part3NativeRender *Part3NativeRender::instance = nullptr;

Part3NativeRender::Part3NativeRender() {
    LOGD("Part3NativeRender create");
}

Part3NativeRender::~Part3NativeRender() {
    LOGD("Part3NativeRender release");
}

Part3NativeRender *Part3NativeRender::GetInstance() {
    if (instance == nullptr) {
        instance = new Part3NativeRender();
    }

    return instance;
}

void Part3NativeRender::OnSurfaceCreate(JNIEnv *env, jobject assetManager) {
    LOGD("Part3NativeRender OnSurfaceCreate");

    //编译链接着色器
    char *vertexSource = readAssetFile(env, assetManager, "glsl/3/vertex.glsl");
    char *fragmentSource = readAssetFile(env, assetManager, "glsl/3/fragment.glsl");

    program = linkShaderProgram(vertexSource, fragmentSource);
    //激活program
    glUseProgram(program);

    //仅使用 VBO
    //VBO();
    //使用 VAO 管理 VBO
    VAO();
}

/**
 * 仅使用 VBO
 */
void Part3NativeRender::VBO() {
    //创建VBO，参数1为VBO个数，参数2为 VBO 指针
    glGenBuffers(1, &vbo);
    //将新建的 Buffer ，绑定到 GL_ARRAY_BUFFER（目标缓冲类型） 上，OpenGL允许同时绑定多个缓冲，只要它们是不同的缓冲类型
    //绑定之后使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /**
     *  顶点数据，glsl/3/vertex.glsl 中的数据如下
     *
     *  |  vertex1  |  vertex2  |  vertex3  |
     *  |X|Y|Z|R|G|B|X|Y|Z|R|G|B|X|Y|Z|R|G|B|
     *  |   0-24    |   24-48   |   48-72   |
     */
    float vertexArray[] = {0.0f, 0.5f, 0.0f,   // top
                           0.0f, 0.0f, 0.0f,   // RGB
                           -0.5f, -0.5f, 0.0f, // bottom left
                           0.0f, 0.5f, 0.5f,   // RGB
                           0.5f, -0.5f, 0.0f,  // bottom right
                           1.0f, 0.25f, 0.25f,   // RGB
    };

    //将顶点数据复制到 Buffer 中
    //参数1：目标缓冲类型
    //参数2：传输数据的大小（单位：字节）
    //参数3：实际发送的数据
    //参数4：希望显卡如何管理数据GL_STATIC_DRAW（数据不会变）、GL_DYNAMIC_DRAW（数据经常改变）、GL_STREAM_DRAW（每次绘制时都会变化）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    //启用顶点属性，顶点属性默认是禁用的
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //告诉OpenGL如何解析数据
    //参数一：顶点属性位置
    //参数二：每个顶点的数据个数
    //参数三：数据类型
    //参数四：是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
    //参数五：步长，连续的顶点属性组之间的间隔（当数值是紧密排列时，可以设置0来让OpenGL决定具体步长是多少）
    //参数六：位置数据在缓冲中起始位置的偏移量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT),
                          (void *) (3 * sizeof(GL_FLOAT)));
}

/**
 * 使用 VAO 管理 VBO，VAO 即 VBO 的数组
 *
 * | VAO1 | VBO1 |
 * | VAO1 | VBO2 |
 * | VAO1 | VBO3 |
 * | VAO1 | VBO4 |
 * | VAO1 | ...  |
 */
void Part3NativeRender::VAO() {
    //创建VAO，参数1为VBO个数，参数2为 VAO 指针
    glGenVertexArrays(1, &vao);
    //绑定VAO，绑定之后使用任何缓冲调用都会保存再当前VAO上
    glBindVertexArray(vao);
    VBO();
}

void Part3NativeRender::OnSurfaceChange(int width, int height) {
    LOGD("Part3NativeRender OnSurfaceChange width = %d height = %d", width, height);
    //设置窗口大小
    glViewport(0, 0, width, height);
}

void Part3NativeRender::OnDrawFrame() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

