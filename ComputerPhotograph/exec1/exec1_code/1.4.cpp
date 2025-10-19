//
// Created by 86180 on 25-10-17.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// 顶点着色器
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)glsl";

// 片段着色器
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 color;
    void main() {
        FragColor = vec4(color, 1.0);
    }
)glsl";

// 立方体顶点数据（用于机器人头部、身体和桌子）
float vertices[] = {
    // 前面
    -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
    // 后面
    -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
    // 左面
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,
    // 右面
     0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
    // 上面
    -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
    // 下面
    -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f
};

// 圆柱体顶点数据（用于四肢，简化版）
float cylinderVertices[] = {
    // 圆柱体侧面（简化，仅用几个顶点表示）
    // 底面圆
    0.0f, -0.5f, 0.0f,  0.5f, -0.5f, 0.0f,  0.35f, -0.5f, 0.35f,
    0.0f, -0.5f, 0.0f,  0.35f, -0.5f, 0.35f,  0.0f, -0.5f, 0.5f,
    // 顶面圆
    0.0f,  0.5f, 0.0f,  0.0f,  0.5f, 0.5f,   0.35f,  0.5f, 0.35f,
    0.0f,  0.5f, 0.0f,  0.35f,  0.5f, 0.35f,  0.5f,  0.5f, 0.0f,
    // 侧面连接
    0.0f, -0.5f, 0.0f,  0.0f,  0.5f, 0.0f,   0.5f,  0.5f, 0.0f,
    0.0f, -0.5f, 0.0f,  0.5f, -0.5f, 0.0f,   0.5f,  0.5f, 0.0f
};

unsigned int VBOs[2], VAOs[2];
unsigned int shaderProgram;

// 初始化着色器
void initShaders() {
    // 编译顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // 检查编译错误（省略详细检查代码以简化）

    // 编译片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 链接着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// 初始化缓冲区
void initBuffers() {
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // 立方体
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 圆柱体
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderVertices), cylinderVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// 绘制立方体
void drawCube(glm::mat4 model, glm::vec3 color) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), color.x, color.y, color.z);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// 绘制圆柱体
void drawCylinder(glm::mat4 model, glm::vec3 color) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), color.x, color.y, color.z);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 18);
}

// 绘制机器人
void drawRobot() {
    // 头部
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    drawCube(model, glm::vec3(0.5f, 0.5f, 0.5f)); // 灰色头部

    // 眼睛（小立方体）
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 2.2f, 0.6f));
    model = glm::scale(model, glm::vec3(0.2f));
    drawCube(model, glm::vec3(1.0f, 1.0f, 1.0f)); // 白色眼睛
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 2.2f, 0.6f));
    model = glm::scale(model, glm::vec3(0.2f));
    drawCube(model, glm::vec3(1.0f, 1.0f, 1.0f));

    // 鼻子（小立方体）
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.6f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.4f));
    drawCube(model, glm::vec3(1.0f, 0.5f, 0.0f)); // 橙色鼻子

    // 嘴巴（扁平立方体）
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.8f, 0.6f));
    model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.1f));
    drawCube(model, glm::vec3(0.0f, 0.0f, 0.0f)); // 黑色嘴巴

    // 身体
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.0f));
    drawCube(model, glm::vec3(0.3f, 0.3f, 0.3f)); // 深灰色身体

    // 左臂
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.5f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.4f, 2.0f, 0.4f));
    drawCylinder(model, glm::vec3(0.4f, 0.4f, 0.4f));

    // 右臂
    model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.4f, 2.0f, 0.4f));
    drawCylinder(model, glm::vec3(0.4f, 0.4f, 0.4f));

    // 左腿
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.6f, 3.0f, 0.6f));
    drawCylinder(model, glm::vec3(0.4f, 0.4f, 0.4f));

    // 右腿
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.6f, 3.0f, 0.6f));
    drawCylinder(model, glm::vec3(0.4f, 0.4f, 0.4f));
}

// 绘制桌子和茶壶
void drawTableAndTeapot() {
    // 桌面
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 0.2f, 2.0f));
    drawCube(model, glm::vec3(0.6f, 0.3f, 0.0f)); // 棕色桌面

    // 桌腿
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.2f, -1.0f, 0.8f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 2.0f, 0.2f));
    drawCylinder(model, glm::vec3(0.4f, 0.2f, 0.0f));

    model = glm::translate(glm::mat4(1.0f), glm::vec3(3.8f, -1.0f, 0.8f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 2.0f, 0.2f));
    drawCylinder(model, glm::vec3(0.4f, 0.2f, 0.0f));

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.2f, -1.0f, -0.8f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 2.0f, 0.2f));
    drawCylinder(model, glm::vec3(0.4f, 0.2f, 0.0f));

    model = glm::translate(glm::mat4(1.0f), glm::vec3(3.8f, -1.0f, -0.8f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 2.0f, 0.2f));
    drawCylinder(model, glm::vec3(0.4f, 0.2f, 0.0f));

    // 茶壶（暂用立方体代替）
    model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.3f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    drawCube(model, glm::vec3(0.0f, 0.5f, 1.0f)); // 蓝色茶壶
}

void display(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置视图和投影矩阵
    glm::mat4 view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    drawRobot();
    drawTableAndTeapot();

    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 创建窗口
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Robot and Teapot", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置视口和深度测试
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化着色器和缓冲区
    initShaders();
    initBuffers();

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        display(window);
        glfwPollEvents();
    }

    // 清理资源
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
