#include <GL/glut.h>
#include <cmath>
#include <iostream>

// 相机参数
float camX = 3.0f, camY = 2.5f, camZ = 5.0f;
float lookX = 0.0f, lookY = 0.0f, lookZ = 0.0f;
float angleH = 0.0f, angleV = 0.0f;  // 水平、垂直旋转角
bool mouseDown = false;
int lastX, lastY;

// 初始化灯光（三盏方向光）
void initLights() {
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    // 红色光：正右上方30°
    GLfloat light0_pos[] = {1.0f, 0.0f, tanf(30.0f * 3.14159f / 180.0f), 0.0f};
    GLfloat light0_diff[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light0_amb[]  = {0.1f, 0.0f, 0.0f, 1.0f};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_amb);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_diff);

    // 蓝色光：左前上方45°
    float a45 = 45.0f * 3.14159f / 180.0f;
    GLfloat light1_pos[] = {-sinf(a45), cosf(a45), sinf(a45), 0.0f};
    GLfloat light1_diff[] = {0.0f, 0.0f, 1.0f, 1.0f};
    GLfloat light1_amb[]  = {0.0f, 0.0f, 0.1f, 1.0f};

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diff);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_amb);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_diff);

    // 黄色光：左前下方60°
    float a60 = 60.0f * 3.14159f / 180.0f;
    GLfloat light2_pos[] = {-sinf(a60), -cosf(a60), sinf(a60), 0.0f};
    GLfloat light2_diff[] = {1.0f, 1.0f, 0.0f, 1.0f};
    GLfloat light2_amb[]  = {0.1f, 0.1f, 0.0f, 1.0f};

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_diff);
    glLightfv(GL_LIGHT2, GL_AMBIENT,  light2_amb);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_diff);
}

// 设置茶壶材质
void setTeapotMaterial() {
    // 环境光：白色，强度 0.2
    GLfloat mat_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};

    // 漫反射：紫色，强度 0.8
    GLfloat mat_diffuse[] = {0.8f, 0.0f, 0.8f, 1.0f};

    // 镜面反射：品红，强度 0.5
    GLfloat mat_specular[] = {0.5f, 0.0f, 0.5f, 1.0f};

    // 自发光：中等强度（发紫光）
    GLfloat mat_emission[] = {0.5f, 0.0f, 0.5f, 1.0f};

    // 光泽度
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  mat_emission);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 计算相机位置（球坐标）
    float radius = sqrtf(camX*camX + camY*camY + camZ*camZ);
    float theta = atan2f(camX, camZ);      // 水平角
    float phi   = acosf(camY / radius);    // 垂直角

    // 应用鼠标旋转
    theta += angleH * 3.14159f / 180.0f;
    phi   += angleV * 3.14159f / 180.0f;

    // 限制垂直角
    if (phi < 0.1f) phi = 0.1f;
    if (phi > 3.04159f) phi = 3.04159f;

    // 更新相机位置
    camX = radius * sinf(phi) * sinf(theta);
    camY = radius * cosf(phi);
    camZ = radius * sinf(phi) * cosf(theta);

    gluLookAt(camX, camY, camZ,
              lookX, lookY, lookZ,
              0.0f, 1.0f, 0.0f);

    // 绘制茶壶前设置材质
    setTeapotMaterial();
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

// 键盘控制移动
void keyboard(unsigned char key, int x, int y) {
    float speed = 0.3f;
    float dx = -camX + lookX;
    float dz = -camZ + lookZ;
    float len = sqrtf(dx*dx + dz*dz);
    if (len > 0.01f) { dx /= len; dz /= len; }

    switch (key) {
        case 27: case 'q': case 'Q': exit(0); break;
        case 'w': case 'W': camZ -= speed; break;
        case 's': case 'S': camZ += speed; break;
        case 'a': case 'A': camX -= speed; break;
        case 'd': case 'D': camX += speed; break;
        case 'e': camY += speed; break;
        case 'c': camY -= speed; break;
        case 'r': case 'R':
            camX = 3.0f; camY = 2.5f; camZ = 5.0f;
            angleH = angleV = 0.0f;
            break;
    }
    glutPostRedisplay();
}

// 鼠标拖动旋转视角
void motion(int x, int y) {
    if (mouseDown) {
        angleH += (x - lastX) * 0.5f;
        angleV += (y - lastY) * 0.5f;
        lastX = x; lastY = y;
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseDown = true;
            lastX = x; lastY = y;
        } else {
            mouseDown = false;
        }
    }
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutCreateWindow("OpenGL 茶壶 - 自定义材质 + 三色光 + 交互观察");

    initLights();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    std::cout << "\n=== OpenGL 茶壶光照材质演示 ===\n";
    std::cout << "材质参数：\n";
    std::cout << "  漫反射：紫色 (0.8, 0.0, 0.8)\n";
    std::cout << "  环境光：白色 强度 0.2\n";
    std::cout << "  镜面反射：品红 强度 0.5，光泽度 50\n";
    std::cout << "  自发光：强度 0.5（发紫光）\n";
    std::cout << "\n操作说明：\n";
    std::cout << "  WASD：前后左右移动\n";
    std::cout << "  E/C：上升/下降\n";
    std::cout << "  鼠标左键拖动：旋转视角\n";
    std::cout << "  R：复位视角\n";
    std::cout << "  Esc：退出\n\n";

    glutMainLoop();
    return 0;
}
