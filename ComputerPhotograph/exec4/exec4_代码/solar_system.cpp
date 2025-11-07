#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>

const unsigned int SCR_WIDTH  = 1000;
const unsigned int SCR_HEIGHT = 800;

float earthOrbitAngle = 0.0f;   // 地球公转角度
float moonOrbitAngle  = 0.0f;   // 月球自转角度

// ---------- 绘制球体 ----------
void drawSphere(float radius, int sectors, int stacks)
{
    const float PI = 3.14159265359f;
    for (int i = 0; i <= stacks; ++i) {
        float lat0 = PI * (-0.5f + (float)(i - 1) / stacks);
        float z0  = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = PI * (-0.5f + (float)i / stacks);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= sectors; ++j) {
            float lng = 2 * PI * (float)j / sectors;
            float x = cos(lng);
            float y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);

            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();
    }
}

// ---------- 渲染一帧 ----------
void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 相机
    gluLookAt(0, 10, 30,   // eye
              0, 0, 0,     // center
              0, 1, 0);    // up

    // 光源（太阳位于原点）
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {0,0,0,1};
    GLfloat light_amb[] = {0.2,0.2,0.2,1};
    GLfloat light_dif[] = {1,1,1,1};
    GLfloat light_spe[] = {1,1,1,1};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);

    // ---------- 太阳（自发光） ----------
    glPushMatrix();
    {
        GLfloat sun_emission[] = {0.8,0.2,0.1,1};
        GLfloat sun_diffuse[]  = {1.0,0.3,0.1,1};
        GLfloat sun_specular[] = {1,1,1,1};
        GLfloat sun_shininess[] = {50};

        glMaterialfv(GL_FRONT, GL_EMISSION,  sun_emission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   sun_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  sun_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, sun_shininess);

        drawSphere(3.0f, 40, 40);
    }
    glPopMatrix();

    // 关闭自发光
    GLfloat no_emission[] = {0,0,0,1};
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

    // ---------- 地球 ----------
    const float earthDist = 12.0f;
    float ex = earthDist * cos(earthOrbitAngle * M_PI / 180.0f);
    float ez = earthDist * sin(earthOrbitAngle * M_PI / 180.0f);

    glPushMatrix();
    glTranslatef(ex, 0, ez);

    {
        // 修改地球颜色为蓝色
        GLfloat earth_diffuse[]  = {0.1,0.3,0.8,1}; // 蓝
        GLfloat earth_specular[] = {0.8,0.8,0.8,1};
        GLfloat earth_shininess[] = {30};

        glMaterialfv(GL_FRONT, GL_DIFFUSE,   earth_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  earth_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, earth_shininess);

        drawSphere(1.5f, 30, 30);
    }

    // ---------- 月球 ----------
    const float moonDist = 4.0f;
    float mx = moonDist * cos(moonOrbitAngle * M_PI / 180.0f);
    float mz = moonDist * sin(moonOrbitAngle * M_PI / 180.0f);

    glPushMatrix();
    glTranslatef(mx, 0, mz);
    {
        // 修改月球颜色为黄色
        GLfloat moon_diffuse[]  = {0.9,0.8,0.3,1}; // 黄
        GLfloat moon_specular[] = {0.7,0.7,0.7,1};
        GLfloat moon_shininess[] = {20};

        glMaterialfv(GL_FRONT, GL_DIFFUSE,   moon_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  moon_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, moon_shininess);

        drawSphere(0.5f, 20, 20);
    }
    glPopMatrix(); // 月球
    glPopMatrix(); // 地球

    // ---------- 轨道线（辅助） ----------
    glDisable(GL_LIGHTING);
    glColor3f(0.3,0.3,0.3);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 5) {
        float a = i * M_PI / 180.0f;
        glVertex3f(earthDist * cos(a), 0, earthDist * sin(a));
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// ---------- 窗口大小回调 ----------
void framebuffer_size_callback(GLFWwindow* win, int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/(double)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// ========================================
int main()
{
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* win = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                     "Solar System (No GLAD)", nullptr, nullptr);
    if (!win) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    // ---------- OpenGL 基础设置 ----------
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)SCR_WIDTH/SCR_HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    double last = glfwGetTime();
    while (!glfwWindowShouldClose(win)) {
        double now = glfwGetTime();
        double dt  = now - last;
        last = now;

        // 1 年 = 12 个月 → 30°/秒
        earthOrbitAngle = fmodf(earthOrbitAngle + 30.0f * dt, 360.0f);
        // 1 个月 = 1 圈 → 360°/秒
        moonOrbitAngle  = fmodf(moonOrbitAngle  + 360.0f * dt, 360.0f);

        render();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}