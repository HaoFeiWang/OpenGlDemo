//
// Created by 王浩飞 on 2021/12/18.
//
#define STB_IMAGE_IMPLEMENTATION

#include "../utils/log.h"
#include "../utils/gl_utils.h"
#include "../utils/asset_utils.h"
#include "../utils/stb_image.h"
#include "Part5NativeRender.h"

Part5NativeRender *Part5NativeRender::instance = nullptr;

Part5NativeRender::Part5NativeRender() {
    LOGD("Part5NativeRender create");
}

Part5NativeRender::~Part5NativeRender() {
    LOGD("Part5NativeRender release");
}

Part5NativeRender *Part5NativeRender::GetInstance() {
    if (instance == nullptr) {
        instance = new Part5NativeRender();
    }

    return instance;
}

void Part5NativeRender::OnSurfaceCreate(JNIEnv *env, jobject assetManager) {
    LOGD("Part5NativeRender OnSurfaceCreate");

    //获取最大支持的顶点个数
    int vertexLocationSize;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexLocationSize);
    LOGD("Part5NativeRender vertexLocationSize = %d", vertexLocationSize);

    //编译链接着色器
    char *vertexSource = readAssetFile(env, assetManager, "glsl/part5/vertex.glsl");
    char *fragmentSource = readAssetFile(env, assetManager, "glsl/part5/fragment.glsl");
    program = linkShaderProgram(vertexSource, fragmentSource);
    glUseProgram(program);

    createVAO();
    createVBO();
    createEBO();
    createTexture(env, assetManager);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                          (void *) (3 * sizeof(GL_FLOAT)));
}

void Part5NativeRender::createVAO() {
    //创建VAO，参数1为VBO个数，参数2为 VAO 指针
    glGenVertexArrays(1, &vao);
    //绑定VAO，绑定之后使用任何缓冲调用都会保存再当前VAO上
    glBindVertexArray(vao);
}

void Part5NativeRender::createVBO() {
    //创建VBO，参数1为VBO个数，参数2为 VBO 指针
    glGenBuffers(1, &vbo);
    //将新建的 Buffer ，绑定到 GL_ARRAY_BUFFER（目标缓冲类型） 上，OpenGL允许同时绑定多个缓冲，只要它们是不同的缓冲类型
    //绑定之后使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /**
     *  顶点数据，glsl/3/vertex.glsl 中的数据如下
     *
     *  | vertex1 | vertex2 | vertex3 |
     *  |X|Y|Z|S|T|X|Y|Z|S|T|X|Y|Z|S|T|
     *  |   0-20  |  20-40  |  40-60  |
     */
    float vertexArray[] = {-1.0f, 0.5f, 0.0f,   // top
                           0.0f, 1.0f,         // ST
                           -1.0f, -0.5f, 0.0f, // bottom left
                           0.0f, 0.0f,         // ST
                           1.0f, -0.5f, 0.0f,  // bottom right
                           1.0f, 0.0f,         // ST
                           1.0f, 0.5f, 0.0f,  // bottom right
                           1.0f, 1.0f,         // ST
    };
    LOGD("vertexArray size = %ld", sizeof(vertexArray));
    //将顶点数据复制到 Buffer 中
    //参数1：目标缓冲类型
    //参数2：传输数据的大小（单位：字节）
    //参数3：实际发送的数据
    //参数4：希望显卡如何管理数据GL_STATIC_DRAW（数据不会变）、GL_DYNAMIC_DRAW（数据经常改变）、GL_STREAM_DRAW（每次绘制时都会变化）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
}

void Part5NativeRender::createEBO() {
    // 注意索引从0开始
    unsigned int indices[] = {
            0, 1, 2, // 第一个三角形
            0, 2, 3  // 第二个三角形
    };
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Part5NativeRender::createTexture(JNIEnv *env, jobject assetManager) {
    //读取纹理图片
    int bufferSize;
    char *fileBuffer = readAssetFile(env, assetManager, "image/texture_wood.jpg", &bufferSize);

    int width;
    int height;
    int channel;
    //从内存中读取图片
    stbi_uc *textureImg = stbi_load_from_memory((stbi_uc *) fileBuffer, bufferSize,
                                                &width, &height, &channel, 0);
    if (textureImg != nullptr) {
        LOGD("Part5NativeRender image width = %d height = %d channel = %d", width, height, channel);
    }

    //激活纹理单元（GL_TEXTURE0 默认是激活状态，可以不用执行）
    glActiveTexture(GL_TEXTURE0);
    //创建纹理
    glGenTextures(1, &textureId);
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D, textureId);
    //设置纹理S轴（横轴）的拉伸方式为截取
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //设置纹理T轴（竖轴）的拉伸方式为截取
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //设置纹理采样方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg);
    //为当前绑定的纹理自动生成所有需要的多级渐远纹理
    glGenerateMipmap(GL_TEXTURE_2D);

    //GLint textureSample = glGetUniformLocation(program, "texture_sample");
    //glUniform1i(textureSample, 0);
}

void Part5NativeRender::OnSurfaceChange(int width, int height) {
    LOGD("Part5NativeRender OnSurfaceChange width = %d height = %d", width, height);
    //设置窗口大小
    glViewport(0, 0, width, height);
}

void Part5NativeRender::OnDrawFrame() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

