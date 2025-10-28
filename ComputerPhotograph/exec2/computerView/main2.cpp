#include <GL/freeglut.h>
#include <cmath> // For cos, sin

float animationProgress = 0.0f; // 动画进度，0.0到1.0之间
const float totalAnimationTime = 5.0f; // 总动画时长，秒
const float scaleDuration = 2.0f;      // 放大阶段时长，秒
const float rotateDuration = totalAnimationTime - scaleDuration; // 旋转阶段时长，秒

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix(); // 保存初始矩阵状态

    float currentScale = 1.0f; // 默认缩放
    float currentAngle = 0.0f; // 默认角度
    float orbitRadius = 0.5f;  // 圆周半径

    // --- 计算当前状态 ---
    float elapsed = fmod(animationProgress, totalAnimationTime); // 循环时间，防止溢出

    if (elapsed <= scaleDuration) {
        // --- 阶段1: 放大 ---
        // 计算当前放大倍数 (从1.0线性变化到2.0)
        currentScale = 1.0f + (2.0f - 1.0f) * (elapsed / scaleDuration);
    } else {
        // --- 阶段2: 旋转 ---
        // 缩放保持在2.0
        currentScale = 2.0f;
        // 计算当前旋转角度 (从0度变化到60度)
        float elapsedRotate = elapsed - scaleDuration; // 旋转阶段经过的时间
        currentAngle = (60.0f * M_PI / 180.0f) * (elapsedRotate / rotateDuration); // 转换为弧度
    }

    // --- 应用变换 ---
    // 1. 应用当前计算出的缩放
    glScalef(currentScale, currentScale, currentScale);

    // 2. 如果在旋转阶段，计算并应用平移到圆周位置的变换
    if (elapsed > scaleDuration) {
        float x_pos = orbitRadius * cos(currentAngle);
        float z_pos = orbitRadius * sin(currentAngle);
        glTranslatef(x_pos, 0.0f, z_pos);
    }
    // 如果仍在放大阶段，立方体中心保持在原点 (0, 0, 0)

    // 3. 绘制立方体 (原始边长0.2)
    glColor3f(1.0f, 1.0f, 1.0f); // White or any color
    glutSolidCube(0.2f); // 原始边长0.2

    glPopMatrix(); // 恢复矩阵状态

    glutSwapBuffers();
}

void idle() {
    // 更新动画进度
    animationProgress += 0.01f; // 每次idle调用增加0.01秒
    glutPostRedisplay(); // 请求重绘
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 5, 0, 0, 0, 0, 1, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Animated Scale then Rotate to 60 Degrees");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle); // 设置空闲回调函数用于动画

    glutMainLoop();
    return 0;
}