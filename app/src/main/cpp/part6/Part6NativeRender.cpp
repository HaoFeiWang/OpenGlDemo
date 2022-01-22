//
// Created by 王浩飞 on 2021/12/18.
//
#define STB_IMAGE_IMPLEMENTATION

#include "../utils/log.h"
#include "../utils/gl_utils.h"
#include "../utils/asset_utils.h"
#include "../utils/stb_image.h"
#include "Part6NativeRender.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtx//string_cast.hpp"

Part6NativeRender *Part6NativeRender::instance = nullptr;

Part6NativeRender::Part6NativeRender() {
    LOGD("Part6NativeRender create");
}

Part6NativeRender::~Part6NativeRender() {
    LOGD("Part6NativeRender release");
}

Part6NativeRender *Part6NativeRender::GetInstance() {
    if (instance == nullptr) {
        instance = new Part6NativeRender();
    }

    return instance;
}

void Part6NativeRender::OnSurfaceCreate(JNIEnv *env, jobject assetManager) {
    LOGD("Part6NativeRender OnSurfaceCreate");
    char *vertexSource = readAssetFile(env, assetManager, "glsl/part6/vertex.glsl");
    char *fragmentSource = readAssetFile(env, assetManager, "glsl/part6/fragment.glsl");
    program = linkShaderProgram(vertexSource, fragmentSource);
    glUseProgram(program);

    createVAO();
    createVBO();
    createEBO();
    createTexture(env, assetManager);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                          (void *) (3 * sizeof(GL_FLOAT)));
}

void Part6NativeRender::createVAO() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void Part6NativeRender::createVBO() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /**
     *  宽1080，高2094，宽高比 0.5158
     *  宽度采用 0.8，则高为0.4126
     *  顶点数据，glsl/3/vertex.glsl 中的数据如下
     *
     *  | vertex1 | vertex2 | vertex3 |
     *  |X|Y|Z|S|T|X|Y|Z|S|T|X|Y|Z|S|T|
     *  |   0-20  |  20-40  |  40-60  |
     */
    float vertexArray[] = {-0.5f, 0.5f, 0.0f,    // top
                           0.0f, 1.0f,           // ST
                           -0.5f, -0.5f, 0.0f,   // bottom left
                           0.0f, 0.0f,           // ST
                           0.5f, -0.5f, 0.0f,    // bottom right
                           1.0f, 0.0f,           // ST
                           0.5f, 0.5f, 0.0f,     // bottom right
                           1.0f, 1.0f,           // ST
                           -0.5f, 0.5f, -1.0f,   // background-top
                           0.0f, 1.0f,           // ST
                           -0.5f, -0.5f, -1.0f,  // background-bottom left
                           0.0f, 0.0f,           // ST
                           0.5f, -0.5f, -1.0f,   // background-bottom right
                           1.0f, 0.0f,           // ST
                           0.5f, 0.5f, -1.0f,    // background-bottom right
                           1.0f, 1.0f            // ST
    };
    LOGD("vertexArray size = %ld", sizeof(vertexArray));
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
}

void Part6NativeRender::createEBO() {
    // 注意索引从0开始
    unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,  // 前
            3, 2, 6,
            3, 6, 7,  // 右
            4, 5, 6,
            4, 6, 7,  // 后
            0, 1, 4,
            1, 4, 5,  // 左
            0, 3, 4,
            3, 4, 7,  // 上
            1, 5, 6,
            1, 6, 2,  // 下
    };
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Part6NativeRender::createTexture(JNIEnv *env, jobject assetManager) {
    //读取纹理图片
    int bufferSize;
    char *fileBuffer = readAssetFile(env, assetManager, "image/texture_wood.jpg", &bufferSize);
    int width;
    int height;
    int channel;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *textureImg = stbi_load_from_memory((stbi_uc *) fileBuffer, bufferSize,
                                                &width, &height, &channel, 0);
    if (textureImg != nullptr) {
        LOGD("Part6NativeRender image width = %d height = %d channel = %d", width, height, channel);
    }

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId1);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg);
    glGenerateMipmap(GL_TEXTURE_2D);

    //读取纹理图片
    int bufferSize2;
    char *fileBuffer2 = readAssetFile(env, assetManager, "image/texture_girl.jpg", &bufferSize2);
    int width2;
    int height2;
    int channel2;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *textureImg2 = stbi_load_from_memory((stbi_uc *) fileBuffer2, bufferSize2,
                                                 &width2, &height2, &channel2, 0);
    if (textureImg2 != nullptr) {
        LOGD("Part6NativeRender image width = %d height = %d channel = %d", width2, height2,
             channel2);
    }

    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &textureId2);
    glBindTexture(GL_TEXTURE_2D, textureId2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 textureImg2);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLint textureSample1 = glGetUniformLocation(program, "texture_sample1");
    glUniform1i(textureSample1, 0);

    GLint textureSample2 = glGetUniformLocation(program, "texture_sample2");
    glUniform1i(textureSample2, 1);
}

/**
 * 创建 3D 效果
 */
void Part6NativeRender::create3d(float width, float height) {
    //绕X轴旋转-55度
    glm::mat4 rotateMat;
    rotateMat = glm::rotate(rotateMat, glm::radians(-0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    //向Z轴反方向移动-3
    glm::mat4 translateMat;
    translateMat = glm::translate(translateMat, glm::vec3(0.0f, 0.0f, -3.0f));

    //创建透视投影矩阵
    glm::mat4 perspectiveMat;
    //参数1：视野，一般为45度
    //参数2：视口宽高比
    //参数3：近平面，一般为0.1
    //参数4：远平面，一般为100
    perspectiveMat = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

    GLint rotatePosition = glGetUniformLocation(program, "rotate");
    GLint transformPosition = glGetUniformLocation(program, "transform");
    GLint perspectivePosition = glGetUniformLocation(program, "perspective");

    glUniformMatrix4fv(rotatePosition, 1, GL_FALSE, glm::value_ptr(rotateMat));
    glUniformMatrix4fv(transformPosition, 1, GL_FALSE, glm::value_ptr(translateMat));
    glUniformMatrix4fv(perspectivePosition, 1, GL_FALSE, glm::value_ptr(perspectiveMat));

    //开启深度测试，各个面都会显示纹理
    glEnable(GL_DEPTH_TEST);
}


void Part6NativeRender::OnSurfaceChange(int width, int height) {
    LOGD("Part6NativeRender OnSurfaceChange width = %d height = %d", width, height);
    //设置窗口大小
    glViewport(0, 0, width, width);
    create3d((float) width, (float) width);
}

float translationX = 0.0f;
float translationY = 0.0f;
float translationZ = 0.0f;
float rotateAngle = 0.0f;
float scaleSize = 1.0f;

void Part6NativeRender::OnDrawFrame() {
    /*translationX += 0.0001f;
    translationY += 0.0001f;
    translationZ += 0.0001f;
    rotateAngle += 0.01f;
    scaleSize -= 0.0001f;
    glm::mat4 transformMat;
    //平移
    transformMat = glm::translate(transformMat,
                                  glm::vec3(translationX, translationY, translationZ));
    //针对Z轴旋转，旋转单位弧度
    transformMat = glm::rotate(transformMat, rotateAngle, glm::vec3(1.0f, 0.0f, 0.0f));

    //缩放
    transformMat = glm::scale(transformMat, glm::vec3(scaleSize, scaleSize, scaleSize));

    GLint transform = glGetUniformLocation(program, "transform");
    //参数2为矩阵个数，参数3为是否需要对矩阵进行置换（交换矩阵的行和列）
    glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(transformMat));*/

    //转起来
    /*glm::mat4 rotateMat;
    rotateAngle += 1.0f;
    rotateMat = glm::rotate(rotateMat, glm::radians(rotateAngle), glm::vec3(1.0f, 1.0f, 1.0f));
    GLint rotatePosition = glGetUniformLocation(program, "rotate");
    glUniformMatrix4fv(rotatePosition, 1, GL_FALSE, glm::value_ptr(rotateMat));*/

    glClearColor(0, 0, 0, 0);
    //GL_DEPTH_BUFFER_BIT 清楚深度缓冲（Z缓冲）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Part6NativeRender::OnTranslation(float x, float y, float z) {
    //创建一个向量
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //创建一个单位矩阵
    glm::mat4 trans;
    LOGD("trans1 = %s", glm::to_string(trans).c_str());
    //生成位移矩阵，参数2为需要位移的值
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    LOGD("trans2 = %s", glm::to_string(trans).c_str());
    //执行位移
    vec = trans * vec;
    LOGD("vec x = %f y = %f z = %f w = %f", vec.x, vec.y, vec.z, vec.w);

    LOGD("transform x = %f y = %f z = %f", x, y, z);
    glm::mat4 transformMat;
    transformMat = glm::translate(transformMat, glm::vec3(x, y, z));
    GLint transform = glGetUniformLocation(program, "transform");
    glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(transformMat));
}