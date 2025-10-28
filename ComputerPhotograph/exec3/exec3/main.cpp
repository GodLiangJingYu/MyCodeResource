#include <GL/glut.h>
#include <cmath>
#include <cstdio>

static int g_width = 1280;
static int g_height = 800;
static float g_angle = 0.0f;
static bool g_showGrid = true;
static bool g_showAxes = true;
static bool g_showLights = true;
static bool g_showShadow = true;

inline float deg2rad(float d) { return d * 3.14159265358979323846f / 180.0f; }

// 光源位置存储（用于可视化）
struct LightPos { float x, y, z; float r, g, b; };
static LightPos g_lights[3];

void setDirectionalLikePos(GLenum light, int idx, float azimuthDeg, float elevationDeg,
                           const GLfloat color[4], float R = 10.0f) {
    float az = deg2rad(azimuthDeg);
    float el = deg2rad(elevationDeg);
    GLfloat pos[4] = {
        R * std::cos(el) * std::sin(az),
        R * std::sin(el),
        R * std::cos(el) * std::cos(az),
        1.0f
    };
    g_lights[idx] = {pos[0], pos[1], pos[2], color[0], color[1], color[2]};

    glLightfv(light, GL_DIFFUSE,  color);
    glLightfv(light, GL_SPECULAR, color);
    glLightf(light, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(light, GL_LINEAR_ATTENUATION,    0.0f);
    glLightf(light, GL_QUADRATIC_ATTENUATION, 0.0f);
    glLightfv(light, GL_POSITION, pos);
}

// 绘制地面网格
void drawGrid(float size = 15.0f, int steps = 30) {
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glColor3f(0.3f, 0.3f, 0.35f);

    glBegin(GL_LINES);
    float step = size / steps;
    for (int i = 0; i <= steps; ++i) {
        float pos = -size/2 + i*step;
        // 平行于 X 轴的线
        glVertex3f(-size/2, 0.0f, pos);
        glVertex3f(size/2, 0.0f, pos);
        // 平行于 Z 轴的线
        glVertex3f(pos, 0.0f, -size/2);
        glVertex3f(pos, 0.0f, size/2);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// 绘制坐标轴
void drawAxes(float len = 5.0f) {
    glDisable(GL_LIGHTING);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
    // X轴 - 红色
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0); glVertex3f(len, 0, 0);
    // Y轴 - 绿色
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0, 0); glVertex3f(0, len, 0);
    // Z轴 - 蓝色
    glColor3f(0.0f, 0.5f, 1.0f);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, len);
    glEnd();
    glEnable(GL_LIGHTING);
}

// 绘制光源位置标记
void drawLightMarkers() {
    glDisable(GL_LIGHTING);
    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        glTranslatef(g_lights[i].x, g_lights[i].y, g_lights[i].z);
        glColor3f(g_lights[i].r, g_lights[i].g, g_lights[i].b);
        glutSolidSphere(0.2, 12, 12);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

// 简单地面阴影（投影到 y=0 平面）
void drawShadow() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.4f);

    glPushMatrix();
    // 简单压平投影矩阵（将 y 压为 0.01）
    GLfloat shadowMat[16] = {
        1, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 1, 0,
        0, 0.01f, 0, 1
    };
    glMultMatrixf(shadowMat);
    glRotatef(g_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidTeapot(1.5);
    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void setupLightsAndMaterial() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // 启用多重采样抗锯齿
    glEnable(GL_MULTISAMPLE);

    const GLfloat globalAmbient[4] = {0.15f, 0.15f, 0.15f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    const GLfloat matAmbient[4]  = {0.25f, 0.18f, 0.15f, 1.0f};
    const GLfloat matDiffuse[4]  = {0.80f, 0.50f, 0.30f, 1.0f};
    const GLfloat matSpecular[4] = {0.95f, 0.95f, 0.95f, 1.0f};
    const GLfloat shininess      = 80.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}

void reshape(int w, int h) {
    g_width = (w > 0) ? w : 1;
    g_height = (h > 0) ? h : 1;
    glViewport(0, 0, g_width, g_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, static_cast<double>(g_width)/g_height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 4.0, 12.0,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0);

    const GLfloat red[4]    = {1.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat blue[4]   = {0.0f, 0.4f, 1.0f, 1.0f};
    const GLfloat yellow[4] = {1.0f, 1.0f, 0.0f, 1.0f};

    setDirectionalLikePos(GL_LIGHT0, 0, 90.0f, 30.0f, red);
    setDirectionalLikePos(GL_LIGHT1, 1, 135.0f, 45.0f, blue);
    setDirectionalLikePos(GL_LIGHT2, 2, 135.0f, -60.0f, yellow);

    // 绘制辅助元素
    if (g_showGrid)   drawGrid();
    if (g_showAxes)   drawAxes();
    if (g_showShadow) drawShadow();

    // 绘制茶壶
    glPushMatrix();
    glRotatef(g_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidTeapot(1.5);
    glPopMatrix();

    if (g_showLights) drawLightMarkers();

    glutSwapBuffers();
}

void idle() {
    g_angle += 0.15f;
    if (g_angle > 360.0f) g_angle -= 360.0f;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int) {
    switch(key) {
        case 27: case 'q': case 'Q': std::exit(0); break;
        case 'g': case 'G': g_showGrid = !g_showGrid; break;
        case 'a': case 'A': g_showAxes = !g_showAxes; break;
        case 'l': case 'L': g_showLights = !g_showLights; break;
        case 's': case 'S': g_showShadow = !g_showShadow; break;
        case 'h': case 'H':
            printf("Controls: G=Grid, A=Axes, L=Lights, S=Shadow, Q=Quit\n");
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(g_width, g_height);
    glutCreateWindow("Enhanced Teapot Lighting - GodLiangJingYu");

    setupLightsAndMaterial();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    printf("=== Enhanced Teapot Lighting ===\n");
    printf("Controls: G=Grid, A=Axes, L=Lights, S=Shadow, Q=Quit\n");

    glutMainLoop();
    return 0;
}