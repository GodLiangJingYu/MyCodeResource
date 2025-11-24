// bouncing_spheres.cpp
// 盒子 6 面贴你的照片 + 漫反射 + 球体立体渲染 + 后墙透明 + 两个发光球反弹 + 球球碰撞
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// ----------------------------------------------------------
// 1. 配置
// ----------------------------------------------------------
const int WIDTH = 1200, HEIGHT = 900;
glm::vec3 cameraPos(0.0f, 0.0f, 6.0f);
const float BOX_SIZE = 2.0f;

// ----------------------------------------------------------
// 2. 小球
// ----------------------------------------------------------
struct Ball {
    glm::vec3 pos, vel, color;
    float radius = 0.25f;
};
Ball ball1, ball2;

// ----------------------------------------------------------
// 3. 球球碰撞检测与处理
// ----------------------------------------------------------
void handleBallCollision(Ball& b1, Ball& b2) {
    glm::vec3 delta = b1.pos - b2.pos;
    float dist = glm::length(delta);
    float minDist = b1.radius + b2.radius;

    if (dist < minDist && dist > 0.001f) {
        glm::vec3 normal = glm::normalize(delta);
        float overlap = minDist - dist;
        b1.pos += normal * (overlap * 0.5f);
        b2.pos -= normal * (overlap * 0.5f);

        glm::vec3 relVel = b1.vel - b2.vel;
        float velAlongNormal = glm::dot(relVel, normal);

        if (velAlongNormal > 0) {
            return;
        }

        float restitution = 0.95f;
        glm::vec3 impulse = (1.0f + restitution) * velAlongNormal * normal;

        b1.vel -= impulse;
        b2.vel += impulse;
    }
}

// ----------------------------------------------------------
// 4. 着色器：盒子（支持透明 + 漫反射）
// ----------------------------------------------------------
const char* boxVS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTex;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

const char* boxFS = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D tex;
uniform vec3 lightPos1, lightColor1;
uniform vec3 lightPos2, lightColor2;
uniform vec3 viewPos;
uniform bool isBackFace;

void main() {
    vec3 n = normalize(Normal);
    vec3 texCol = texture(tex, TexCoord).rgb;

    // 计算两个光源的方向和距离
    vec3 toLight1 = lightPos1 - FragPos;
    vec3 toLight2 = lightPos2 - FragPos;
    float dist1 = length(toLight1);
    float dist2 = length(toLight2);
    vec3 l1 = normalize(toLight1);
    vec3 l2 = normalize(toLight2);

    // 距离衰减 (更强的光照)
    float attenuation1 = 8.0 / (1.0 + 0.3 * dist1 + 0.1 * dist1 * dist1);
    float attenuation2 = 8.0 / (1.0 + 0.3 * dist2 + 0.1 * dist2 * dist2);

    // 漫反射 (兰伯特余弦定律)
    float diff1 = max(dot(n, l1), 0.0);
    float diff2 = max(dot(n, l2), 0.0);
    vec3 diffuse = (diff1 * lightColor1 * attenuation1 +
                    diff2 * lightColor2 * attenuation2) * texCol;

    // 镜面反射 (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 h1 = normalize(l1 + viewDir);
    vec3 h2 = normalize(l2 + viewDir);
    float spec1 = pow(max(dot(n, h1), 0.0), 64.0);
    float spec2 = pow(max(dot(n, h2), 0.0), 64.0);
    vec3 specular = 1.5 * (spec1 * lightColor1 * attenuation1 +
                           spec2 * lightColor2 * attenuation2);

    // 环境光
    vec3 ambient = 0.15 * texCol;

    vec3 color = ambient + diffuse + specular;

    float alpha = isBackFace ? 0.2 : 1.0;  // 后墙更透明
    FragColor = vec4(color, alpha);
}
)";

// ----------------------------------------------------------
// 5. 球体着色器（自发光光源）
// ----------------------------------------------------------
const char* ballVS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

const char* ballFS = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 ballColor;
uniform vec3 viewPos;

void main() {
    vec3 n = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 强烈的自发光
    vec3 emission = ballColor * 2.0;

    // 边缘发光效果 (Fresnel)
    float fresnel = pow(1.0 - max(dot(n, viewDir), 0.0), 3.0);
    vec3 glow = ballColor * fresnel * 1.5;

    // 立体感高光
    vec3 lightDir = normalize(vec3(0.5, 1.0, 1.0));
    float spec = pow(max(dot(reflect(-lightDir, n), viewDir), 0.0), 32.0);
    vec3 specular = vec3(1.0) * spec * 0.5;

    vec3 color = emission + glow + specular;
    FragColor = vec4(color, 1.0);
}
)";

// ----------------------------------------------------------
// 6. 编译着色器
// ----------------------------------------------------------
unsigned int compile(GLenum type, const char* src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int ok; glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        cerr << "Shader Error: " << log << endl;
    }
    return id;
}

unsigned int progBox() {
    unsigned int p = glCreateProgram();
    glAttachShader(p, compile(GL_VERTEX_SHADER, boxVS));
    glAttachShader(p, compile(GL_FRAGMENT_SHADER, boxFS));
    glLinkProgram(p);
    return p;
}

unsigned int progBall() {
    unsigned int p = glCreateProgram();
    glAttachShader(p, compile(GL_VERTEX_SHADER, ballVS));
    glAttachShader(p, compile(GL_FRAGMENT_SHADER, ballFS));
    glLinkProgram(p);
    return p;
}

// ----------------------------------------------------------
// 7. 盒子顶点生成（修复版）
// ----------------------------------------------------------
vector<float> boxVertices;
vector<bool> faceIsBack;

void addQuad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal, bool isBack) {
    // 两个三角形组成一个四边形
    glm::vec3 verts[6] = {p0, p1, p2, p2, p3, p0};
    glm::vec2 texs[6] = {{0,0}, {1,0}, {1,1}, {1,1}, {0,1}, {0,0}};

    for (int i = 0; i < 6; ++i) {
        boxVertices.push_back(verts[i].x);
        boxVertices.push_back(verts[i].y);
        boxVertices.push_back(verts[i].z);
        boxVertices.push_back(normal.x);
        boxVertices.push_back(normal.y);
        boxVertices.push_back(normal.z);
        boxVertices.push_back(texs[i].x);
        boxVertices.push_back(texs[i].y);
        if (i == 0) faceIsBack.push_back(isBack);
    }
}

void initBox() {
    float s = 1.0f;

    // 后墙 (透明, z = -s, 朝向 +z)
    addQuad(glm::vec3(-s, -s, -s), glm::vec3(s, -s, -s),
            glm::vec3(s, s, -s), glm::vec3(-s, s, -s),
            glm::vec3(0, 0, 1), true);

    // 前墙 (z = s, 朝向 -z, 不渲染或注释掉让盒子开口)
    // addQuad(glm::vec3(-s, -s, s), glm::vec3(-s, s, s),
    //         glm::vec3(s, s, s), glm::vec3(s, -s, s),
    //         glm::vec3(0, 0, -1), false);

    // 左墙 (x = -s, 朝向 +x)
    addQuad(glm::vec3(-s, -s, -s), glm::vec3(-s, s, -s),
            glm::vec3(-s, s, s), glm::vec3(-s, -s, s),
            glm::vec3(1, 0, 0), false);

    // 右墙 (x = s, 朝向 -x)
    addQuad(glm::vec3(s, -s, -s), glm::vec3(s, -s, s),
            glm::vec3(s, s, s), glm::vec3(s, s, -s),
            glm::vec3(-1, 0, 0), false);

    // 底墙 (y = -s, 朝向 +y)
    addQuad(glm::vec3(-s, -s, -s), glm::vec3(-s, -s, s),
            glm::vec3(s, -s, s), glm::vec3(s, -s, -s),
            glm::vec3(0, 1, 0), false);

    // 顶墙 (y = s, 朝向 -y)
    addQuad(glm::vec3(-s, s, -s), glm::vec3(s, s, -s),
            glm::vec3(s, s, s), glm::vec3(-s, s, s),
            glm::vec3(0, -1, 0), false);
}

// ----------------------------------------------------------
// 8. 加载纹理
// ----------------------------------------------------------
unsigned int loadTex(const char* path) {
    unsigned int t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);

    int w, h, c;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* d = stbi_load(path, &w, &h, &c, 0);

    if (!d) {
        cerr << "Failed to load texture: " << path << endl;
        // 创建一个简单的棋盘格纹理作为后备
        unsigned char checkerboard[64*64*3];
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                unsigned char color = ((i/8 + j/8) % 2) ? 200 : 100;
                checkerboard[(i*64+j)*3+0] = color;
                checkerboard[(i*64+j)*3+1] = color;
                checkerboard[(i*64+j)*3+2] = color;
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerboard);
    } else {
        GLenum fmt = (c == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, d);
        stbi_image_free(d);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return t;
}

// ----------------------------------------------------------
// 9. 球体网格生成
// ----------------------------------------------------------
void genSphere(float r, int sub, vector<float>& v, vector<unsigned int>& i) {
    const float X = 0.525731112119133606f, Z = 0.850650808352039932f;
    glm::vec3 vd[12] = {
        {-X,0,Z},{X,0,Z},{-X,0,-Z},{X,0,-Z},
        {0,Z,X},{0,Z,-X},{0,-Z,X},{0,-Z,-X},
        {Z,X,0},{-Z,X,0},{Z,-X,0},{-Z,-X,0}
    };
    unsigned int id[60] = {
        0,4,1,0,9,4,9,5,4,4,5,8,4,8,1,
        8,10,1,8,3,10,5,3,8,5,2,3,2,7,3,
        7,10,3,7,6,10,7,11,6,11,0,6,0,1,6,
        6,1,10,9,0,11,9,11,2,9,2,5,7,2,11
    };

    for (int j = 0; j < 60; j += 3) {
        i.push_back(id[j]);
        i.push_back(id[j+1]);
        i.push_back(id[j+2]);
    }

    for (int j = 0; j < 12; ++j) {
        glm::vec3 p = glm::normalize(vd[j]) * r;
        v.insert(v.end(), {p.x, p.y, p.z, p.x, p.y, p.z});
    }

    for (int s = 0; s < sub; ++s) {
        vector<unsigned int> ni;
        for (size_t j = 0; j < i.size(); j += 3) {
            unsigned int a = i[j], b = i[j+1], c = i[j+2];
            unsigned int ab = v.size()/6, bc = ab+1, ca = ab+2;

            glm::vec3 va = glm::make_vec3(&v[a*6]);
            glm::vec3 vb = glm::make_vec3(&v[b*6]);
            glm::vec3 vc = glm::make_vec3(&v[c*6]);

            glm::vec3 mab = glm::normalize(va + vb) * r;
            glm::vec3 mbc = glm::normalize(vb + vc) * r;
            glm::vec3 mca = glm::normalize(vc + va) * r;

            v.insert(v.end(), {mab.x, mab.y, mab.z, mab.x, mab.y, mab.z});
            v.insert(v.end(), {mbc.x, mbc.y, mbc.z, mbc.x, mbc.y, mbc.z});
            v.insert(v.end(), {mca.x, mca.y, mca.z, mca.x, mca.y, mca.z});

            ni.insert(ni.end(), {a,ab,ca, ab,b,bc, ca,ab,bc, bc,c,ca});
        }
        i = ni;
    }
}

// ----------------------------------------------------------
// 10. 主函数
// ----------------------------------------------------------
int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, "Your Photo Box with Bouncing Light Spheres", nullptr, nullptr);
    if (!win) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    unsigned int shaderBox = progBox();
    unsigned int shaderBall = progBall();

    // 加载纹理
    unsigned int tex = loadTex("your_photo.jpg");

    // 初始化盒子
    initBox();

    unsigned int boxVAO, boxVBO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);
    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, boxVertices.size() * sizeof(float), boxVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // 生成球体
    vector<float> sphV;
    vector<unsigned int> sphI;
    genSphere(0.25f, 3, sphV, sphI);

    unsigned int sphVAO, sphVBO, sphEBO;
    glGenVertexArrays(1, &sphVAO);
    glGenBuffers(1, &sphVBO);
    glGenBuffers(1, &sphEBO);
    glBindVertexArray(sphVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphVBO);
    glBufferData(GL_ARRAY_BUFFER, sphV.size() * sizeof(float), sphV.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphI.size() * sizeof(unsigned int), sphI.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // 初始化球体
    ball1 = {{-0.8f, 0.5f, 0.3f}, {1.2f, 0.8f, 0.5f}, {1.0f, 0.2f, 0.2f}};  // 红色
    ball2 = {{ 0.6f, -0.6f, -0.4f}, {-0.9f, 1.1f, -0.8f}, {0.2f, 1.0f, 0.2f}}; // 绿色

    float last = glfwGetTime();

    while (!glfwWindowShouldClose(win)) {
        float now = glfwGetTime();
        float dt = now - last;
        last = now;

        // 物理更新
        for (auto* b : {&ball1, &ball2}) {
            b->pos += b->vel * dt;

            // 墙壁碰撞
            for (int axis = 0; axis < 3; ++axis) {
                if (fabs(b->pos[axis]) > BOX_SIZE - b->radius) {
                    b->vel[axis] = -b->vel[axis];
                    b->pos[axis] = glm::sign(b->pos[axis]) * (BOX_SIZE - b->radius);
                }
            }
        }

        // 球球碰撞
        handleBallCollision(ball1, ball2);

        // 清屏
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 矩阵变换
        glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(BOX_SIZE));
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);

        // ========== 绘制盒子 ==========
        glUseProgram(shaderBox);
        glUniformMatrix4fv(glGetUniformLocation(shaderBox, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderBox, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderBox, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniform3fv(glGetUniformLocation(shaderBox, "lightPos1"), 1, glm::value_ptr(ball1.pos));
        glUniform3fv(glGetUniformLocation(shaderBox, "lightColor1"), 1, glm::value_ptr(ball1.color));
        glUniform3fv(glGetUniformLocation(shaderBox, "lightPos2"), 1, glm::value_ptr(ball2.pos));
        glUniform3fv(glGetUniformLocation(shaderBox, "lightColor2"), 1, glm::value_ptr(ball2.color));
        glUniform3fv(glGetUniformLocation(shaderBox, "viewPos"), 1, glm::value_ptr(cameraPos));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(glGetUniformLocation(shaderBox, "tex"), 0);

        glBindVertexArray(boxVAO);
        int numFaces = faceIsBack.size();
        for (int i = 0; i < numFaces; ++i) {
            glUniform1i(glGetUniformLocation(shaderBox, "isBackFace"), faceIsBack[i]);
            glDrawArrays(GL_TRIANGLES, i * 6, 6);
        }

        // ========== 绘制球体 ==========
        glUseProgram(shaderBall);
        glUniformMatrix4fv(glGetUniformLocation(shaderBall, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderBall, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniform3fv(glGetUniformLocation(shaderBall, "viewPos"), 1, glm::value_ptr(cameraPos));

        for (auto* b : {&ball1, &ball2}) {
            glm::mat4 m = glm::translate(glm::mat4(1), b->pos);
            glUniformMatrix4fv(glGetUniformLocation(shaderBall, "model"), 1, GL_FALSE, glm::value_ptr(m));
            glUniform3fv(glGetUniformLocation(shaderBall, "ballColor"), 1, glm::value_ptr(b->color));

            glBindVertexArray(sphVAO);
            glDrawElements(GL_TRIANGLES, sphI.size(), GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &boxVAO);
    glDeleteBuffers(1, &boxVBO);
    glDeleteVertexArrays(1, &sphVAO);
    glDeleteBuffers(1, &sphVBO);
    glDeleteBuffers(1, &sphEBO);

    glfwTerminate();
    return 0;
}