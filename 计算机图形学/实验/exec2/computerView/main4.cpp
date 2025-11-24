#include <GL/freeglut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- 步骤1: 设置视口为左下1/4 ---
    int win_width = glutGet(GLUT_WINDOW_WIDTH);
    int win_height = glutGet(GLUT_WINDOW_HEIGHT);
    int sub_width = win_width / 2;
    int sub_height = win_height / 2;
    int sub_x = 0; // 左下角X坐标
    int sub_y = 0; // 左下角Y坐标

    // 启用裁剪测试
    glEnable(GL_SCISSOR_TEST);
    // 设置裁剪区域 (与视口相同，即整个左下1/4)
    glScissor(sub_x, sub_y, sub_width, sub_height);

    // 设置视口
    glViewport(sub_x, sub_y, sub_width, sub_height);

    // 设置投影矩阵 (适用于左下1/4视口)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)sub_width / (double)sub_height, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // --- 步骤2: 绘制茶壶，并裁剪上半部分 ---
    // 为了只显示上半部分，我们需要再次设置裁剪区域
    // 假设茶壶在视口坐标系中大致占据中心区域
    // 我们将裁剪区域设置为左下1/4中的上半部分 (即整个窗口的左下1/8)
    int clip_width = sub_width;
    int clip_height = sub_height / 2; // 只取高度的一半
    int clip_x = sub_x; // 从左下1/4的左下角开始
    int clip_y = sub_y + sub_height / 2; // Y坐标移到左下1/4的中间

    glScissor(clip_x, clip_y, clip_width, clip_height);

    // 清除指定裁剪区域 (可选，用于可视化裁剪边界)
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 绘制茶壶
    glColor3f(1.0f, 1.0f, 1.0f); // White or any color
    glutSolidTeapot(1.0); // 调整大小以适应视口

    // 禁用裁剪测试，恢复全窗口视口用于其他可能的绘制 (如果需要)
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, win_width, win_height);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    // reshape函数在这里不直接处理视口，因为display中会覆盖它
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Viewport and Scissor");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}