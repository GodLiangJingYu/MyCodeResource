#include <GL/glut.h>
#include <cmath>
#include <iostream>

void initLights() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // 光源0：红色光，从正右上方30度
    GLfloat light0_pos[] = { 1.0f, 0.0f, std::tan(30.0f * 3.14159f / 180.0f), 0.0f }; // 方向光
    GLfloat light0_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // 红色漫反射
    GLfloat light0_ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f }; // 微弱环境光

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_diffuse); // 高光也用红色

    // 光源1：蓝色光，从左前上方45度
    // 左前：(-x, +y, +z)，45度俯角
    float angle45 = 45.0f * 3.14159f / 180.0f;
    float x1 = -std::sin(angle45);
    float y1 = std::cos(angle45);
    float z1 = std::sin(angle45);
    GLfloat light1_pos[] = { x1, y1, z1, 0.0f };
    GLfloat light1_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat light1_ambient[] = { 0.0f, 0.0f, 0.1f, 1.0f };

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_diffuse);

    // 光源2：黄色光，从左前下方60度
    // 左前下方：(-x, -y, +z)，60度俯角（从下往上照）
    float angle60 = 60.0f * 3.14159f / 180.0f;
    float x2 = -std::sin(angle60);
    float y2 = -std::cos(angle60);
    float z2 = std::sin(angle60);
    GLfloat light2_pos[] = { x2, y2, z2, 0.0f };
    GLfloat light2_diffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    GLfloat light2_ambient[] = { 0.1f, 0.1f, 0.0f, 1.0f };

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_diffuse);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 相机位置：从斜上方看茶壶
    gluLookAt(2.0, 2.0, 3.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // 绘制实体茶壶
    glutSolidTeapot(1.0);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL 茶壶 - 三色光照");

    initLights();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    std::cout << "程序运行中...\n"
              << "观察茶壶：\n"
              << " - 右上方30°：红色光\n"
              << " - 左前上方45°：蓝色光\n"
              << " - 左前下方60°：黄色光\n"
              << "按 Esc 关闭窗口。\n";

    glutMainLoop();
    return 0;
}