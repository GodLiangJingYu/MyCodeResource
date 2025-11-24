#include <GL/glut.h>

int currentShape = 0; // 0: 无, 1: 红色球体, 2: 绿色圆锥, 3: 蓝色立方体, 4: 紫色茶壶, 5: 车道线
float angle = 0.0; // 旋转角度，用于动态旋转

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置背景为黑色
    glEnable(GL_DEPTH_TEST); // 启用深度测试

    // 设置光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = {2.0, 2.0, 2.0, 1.0}; // 光源位置
    GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0}; // 环境光
    GLfloat diffuse[] = {0.8, 0.8, 0.8, 1.0}; // 漫反射光
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0}; // 镜面反射光
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    // 设置材质
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specMaterial[] = {1.0, 1.0, 1.0, 1.0}; // 高光材质
    glMaterialfv(GL_FRONT, GL_SPECULAR, specMaterial);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0); // 高光指数
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置相机位置，优化视角
    gluLookAt(2.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0); // 绕 y 轴旋转，增强三维感

    switch (currentShape) {
        case 1: // 红色球体
            glColor3f(1.0, 0.0, 0.0); // 红色
            glutSolidSphere(1.0, 32, 32); // 增加细分以平滑
            break;
        case 2: // 绿色圆锥
            glColor3f(0.0, 1.0, 0.0); // 绿色
            glutSolidCone(1.0, 2.0, 32, 32); // 增加细分
            break;
        case 3: // 蓝色立方体
            glColor3f(0.0, 0.0, 1.0); // 蓝色
            glutSolidCube(1.5);
            break;
        case 4: // 紫色茶壶
            glColor3f(1.0, 0.0, 1.0); // 紫色
            glutSolidTeapot(1.0);
            break;
        case 5: // 车道线（3D 细长矩形）
            glPushMatrix();
            glTranslatef(0.0, 0.0, -0.5); // 调整位置
            // 黄色实线（细长立方体）
            glColor3f(1.0, 1.0, 0.0); // 黄色
            glPushMatrix();
            glScalef(4.0, 0.1, 0.1); // 拉伸为细长矩形
            glutSolidCube(1.0);
            glPopMatrix();
            // 左侧白色虚线（细长立方体）
            glColor3f(1.0, 1.0, 1.0); // 白色
            glPushMatrix();
            glTranslatef(0.0, 0.5, 0.0);
            glScalef(4.0, 0.1, 0.1);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x00FF);
            glutWireCube(1.0); // 使用线框模拟虚线
            glDisable(GL_LINE_STIPPLE);
            glPopMatrix();
            // 右侧白色虚线
            glPushMatrix();
            glTranslatef(0.0, -0.5, 0.0);
            glScalef(4.0, 0.1, 0.1);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x00FF);
            glutWireCube(1.0);
            glDisable(GL_LINE_STIPPLE);
            glPopMatrix();
            glPopMatrix();
            break;
        default:
            break;
    }

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            currentShape = 1; // 红色球体
            break;
        case '2':
            currentShape = 2; // 绿色圆锥
            break;
        case '3':
            currentShape = 3; // 蓝色立方体
            break;
        case 27: // ESC 键退出
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_UP) { // 仅在鼠标释放时触发
        if (button == GLUT_LEFT_BUTTON) {
            currentShape = 4; // 紫色茶壶
        } else if (button == GLUT_RIGHT_BUTTON) {
            currentShape = 5; // 车道线
        }
        glutPostRedisplay();
    }
}

void timer(int value) {
    angle += 1.0; // 每帧旋转 1 度
    if (angle >= 360.0) angle -= 360.0; // 循环旋转
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 约 60 FPS
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Program 1: Enhanced 3D Shapes with Keyboard and Mouse");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0); // 启动旋转动画
    glutMainLoop();
    return 0;
}