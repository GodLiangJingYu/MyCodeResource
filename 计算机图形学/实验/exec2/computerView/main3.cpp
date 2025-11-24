#include <GL/freeglut.h>
#include <cmath> // For cos, sin

float animationProgress = 0.0f; // 动画进度，0.0到1.0之间
const float totalAnimationTime = 4.0f; // 总动画时长，秒

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- 设置光照 ---
    GLfloat light_position[] = { 1.0f, 5.0f, 5.0f, 1.0f }; // 光源位置
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // 环境光
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // 漫射光
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 镜面光

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);  // 启用光照
    glEnable(GL_LIGHT0);    // 启用第0号光源
    glEnable(GL_COLOR_MATERIAL); // 启用颜色材质，使glColor影响材质属性
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // 设置颜色影响漫射和环境反射

    glPushMatrix(); // 保存初始矩阵状态

    // --- 计算茶壶当前状态 ---
    float elapsed = fmod(animationProgress, totalAnimationTime); // 循环时间
    float teaPotTargetAngle = -60.0f * M_PI / 180.0f; // 目标角度，顺时针60度 = -60度 (弧度)
    float teaPotTargetX = 0.5f; // 目标平移X
    float teaPotCurrentAngle = teaPotTargetAngle * (elapsed / totalAnimationTime); // 当前角度
    float teaPotCurrentX = teaPotTargetX * (elapsed / totalAnimationTime); // 当前平移X

    glPushMatrix(); // 保存矩阵状态，用于茶壶
    // 1. 顺时针旋转
    glRotatef(teaPotCurrentAngle * 180.0f / M_PI, 0.0f, 1.0f, 0.0f); // 将弧度转回角度
    // 2. 向右平移
    glTranslatef(teaPotCurrentX, 0.0f, 0.0f);
    // 3. 设置颜色并绘制茶壶 (半径0.1对应glutSolidTeapot的参数0.1)
    glColor3f(0.5f, 0.0f, 0.5f); // Purple
    glutSolidTeapot(0.1f); // 半径0.1
    glPopMatrix(); // 恢复矩阵状态

    // --- 计算球当前状态 ---
    float sphereTargetY = 0.2f; // 目标平移Y
    float sphereCurrentY = sphereTargetY * (elapsed / totalAnimationTime); // 当前平移Y

    glPushMatrix(); // 保存矩阵状态，用于球
    // 1. 向上平移
    glTranslatef(0.0f, sphereCurrentY, 0.0f);
    // 2. 设置颜色并绘制球 (半径0.1)
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glutSolidSphere(0.1f, 20, 20); // 半径0.1
    glPopMatrix(); // 恢复矩阵状态

    glPopMatrix(); // 恢复初始矩阵状态

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
    glutCreateWindow("Animated Transform with Lighting");

    glEnable(GL_DEPTH_TEST); // 启用深度测试

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle); // 设置空闲回调函数用于动画

    glutMainLoop();
    return 0;
}