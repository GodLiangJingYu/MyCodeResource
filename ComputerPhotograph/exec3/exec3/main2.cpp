#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static int g_width2 = 1280;
static int g_height2 = 800;
static float g_angle2 = 0.0f;
static bool g_spin = true;

// 观察参数
static float camDist = 12.0f;
static float camYaw  = 135.0f;
static float camPitch= 25.0f;

// 显示开关
static bool g_showGrid = true;
static bool g_showAxes = true;
static bool g_showLights = true;
static bool g_showShadow = true;
static bool g_showWireframe = false;

// 光源位置存储
struct LightPos2 { float x, y, z; float r, g, b; };
static LightPos2 g_lights2[3];

inline float deg2rad2(float d){ return d * 3.14159265358979323846f / 180.0f; }

static void setDirectionalLikePos2(GLenum light, int idx, float azimuthDeg, float elevationDeg,
                                   const GLfloat color[4], float R = 20.0f) {
    const float az = deg2rad2(azimuthDeg);
    const float el = deg2rad2(elevationDeg);
    const GLfloat pos[4] = {
        R * std::cos(el) * std::sin(az),
        R * std::sin(el),
        R * std::cos(el) * std::cos(az),
        1.0f
    };
    g_lights2[idx] = {pos[0], pos[1], pos[2], color[0], color[1], color[2]};

    glLightfv(light, GL_DIFFUSE,  color);
    glLightfv(light, GL_SPECULAR, color);
    glLightf(light, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(light, GL_LINEAR_ATTENUATION,    0.0f);
    glLightf(light, GL_QUADRATIC_ATTENUATION, 0.0f);
    glLightfv(light, GL_POSITION, pos);
}

static void drawGrid2(float size = 20.0f, int steps = 40) {
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);

    glBegin(GL_LINES);
    float step = size / steps;
    for (int i = 0; i <= steps; ++i) {
        float pos = -size/2 + i*step;
        // 中心线加亮
        if (i == steps/2) glColor3f(0.5f, 0.5f, 0.55f);
        else glColor3f(0.25f, 0.25f, 0.3f);

        glVertex3f(-size/2, 0.0f, pos);
        glVertex3f(size/2, 0.0f, pos);
        glVertex3f(pos, 0.0f, -size/2);
        glVertex3f(pos, 0.0f, size/2);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

static void drawAxes2(float len = 6.0f) {
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex3f(0, 0, 0); glVertex3f(len, 0, 0);
    glColor3f(0.2f, 1.0f, 0.2f);
    glVertex3f(0, 0, 0); glVertex3f(0, len, 0);
    glColor3f(0.2f, 0.5f, 1.0f);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, len);
    glEnd();

    // 箭头提示
    glPushMatrix();
    glTranslatef(len, 0, 0);
    glRotatef(90, 0, 1, 0);
    glColor3f(1.0f, 0.2f, 0.2f);
    glutSolidCone(0.15, 0.4, 8, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, len, 0);
    glRotatef(-90, 1, 0, 0);
    glColor3f(0.2f, 1.0f, 0.2f);
    glutSolidCone(0.15, 0.4, 8, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, len);
    glColor3f(0.2f, 0.5f, 1.0f);
    glutSolidCone(0.15, 0.4, 8, 1);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

static void drawLightMarkers2() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < 3; ++i) {
        // 光晕效果
        glPushMatrix();
        glTranslatef(g_lights2[i].x, g_lights2[i].y, g_lights2[i].z);
        glColor4f(g_lights2[i].r, g_lights2[i].g, g_lights2[i].b, 0.3f);
        glutSolidSphere(0.5, 16, 16);
        glColor3f(g_lights2[i].r, g_lights2[i].g, g_lights2[i].b);
        glutSolidSphere(0.25, 12, 12);
        glPopMatrix();

        // 连线到原点
        glLineWidth(1.5f);
        glColor4f(g_lights2[i].r, g_lights2[i].g, g_lights2[i].b, 0.4f);
        glBegin(GL_LINES);
        glVertex3f(0, 0.5f, 0);
        glVertex3f(g_lights2[i].x, g_lights2[i].y, g_lights2[i].z);
        glEnd();
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

static void drawShadow2() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

    glPushMatrix();
    GLfloat shadowMat[16] = {
        1, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 1, 0,
        0, 0.02f, 0, 1
    };
    glMultMatrixf(shadowMat);
    glRotatef(g_angle2, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidTeapot(1.5);
    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

static void setupLightsAndMaterial2() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_MULTISAMPLE);

    const GLfloat globalAmbient[4] = {0.08f, 0.08f, 0.08f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // 题目材质要求
    const GLfloat matDiffuse[4]  = {0.8f, 0.0f, 0.8f, 1.0f};  // 紫色漫反射 0.8
    const GLfloat matAmbient[4]  = {0.2f, 0.2f, 0.2f, 1.0f};  // 白色环境 0.2
    const GLfloat matSpecular[4] = {0.5f, 0.0f, 0.5f, 1.0f};  // 品红镜面 0.5
    const GLfloat shininess      = 50.0f;                      // 光泽度 50
    const GLfloat matEmission[4] = {0.5f, 0.5f, 0.5f, 1.0f};  // 自发光 0.5

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  matSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  matEmission);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}

static void reshape2(int w, int h) {
    g_width2  = (w > 0) ? w : 1;
    g_height2 = (h > 0) ? h : 1;
    glViewport(0, 0, g_width2, g_height2);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, static_cast<double>(g_width2)/g_height2, 0.1, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void applyCamera() {
    const float ry = deg2rad2(camYaw);
    const float rp = deg2rad2(camPitch);
    const float cx = camDist * std::cos(rp) * std::sin(ry);
    const float cy = camDist * std::sin(rp);
    const float cz = camDist * std::cos(rp) * std::cos(ry);
    gluLookAt(cx, cy, cz,  0.0, 0.8, 0.0,  0.0, 1.0, 0.0);
}

static void display2() {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    applyCamera();

    const GLfloat red   [4] = {1.0f, 0.0f, 0.0f, 1.0f};
    const GLfloat blue  [4] = {0.0f, 0.4f, 1.0f, 1.0f};
    const GLfloat yellow[4] = {1.0f, 1.0f, 0.0f, 1.0f};

    setDirectionalLikePos2(GL_LIGHT0, 0, 90.0f, 30.0f, red);
    setDirectionalLikePos2(GL_LIGHT1, 1, 135.0f, 45.0f, blue);
    setDirectionalLikePos2(GL_LIGHT2, 2, 135.0f, -60.0f, yellow);

    if (g_showGrid)   drawGrid2();
    if (g_showAxes)   drawAxes2();
    if (g_showShadow) drawShadow2();

    // 茶壶
    glPushMatrix();
    if (g_spin) {
        g_angle2 += 0.2f;
        if (g_angle2 > 360.0f) g_angle2 -= 360.0f;
    }
    glRotatef(g_angle2, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);

    if (g_showWireframe) {
        glDisable(GL_LIGHTING);
        glColor3f(0.8f, 0.0f, 0.8f);
        glutWireTeapot(1.5);
        glEnable(GL_LIGHTING);
    } else {
        glutSolidTeapot(1.5);
    }
    glPopMatrix();

    if (g_showLights) drawLightMarkers2();

    glutSwapBuffers();
}

static void idle2() {
    glutPostRedisplay();
}

static void keyboard2(unsigned char key, int, int) {
    switch (key) {
        case 27: case 'q': case 'Q': std::exit(0); break;
        case '+': case '=': camDist = std::max(2.0f, camDist - 0.5f); break;
        case '-': case '_': camDist = std::min(80.0f, camDist + 0.5f); break;
        case ' ': g_spin = !g_spin; break;
        case 'g': case 'G': g_showGrid = !g_showGrid; break;
        case 'a': case 'A': g_showAxes = !g_showAxes; break;
        case 'l': case 'L': g_showLights = !g_showLights; break;
        case 's': case 'S': g_showShadow = !g_showShadow; break;
        case 'w': case 'W': g_showWireframe = !g_showWireframe; break;
        case 'r': case 'R':
            camDist = 12.0f; camYaw = 135.0f; camPitch = 25.0f;
            printf("Camera reset.\n");
            break;
        case 'h': case 'H':
            printf("\n=== Controls ===\n");
            printf("Arrow Keys: Rotate view (Yaw/Pitch)\n");
            printf("+/- : Zoom in/out\n");
            printf("Space: Pause/Resume spin\n");
            printf("G: Toggle grid\n");
            printf("A: Toggle axes\n");
            printf("L: Toggle light markers\n");
            printf("S: Toggle shadow\n");
            printf("W: Toggle wireframe\n");
            printf("R: Reset camera\n");
            printf("Q: Quit\n\n");
            break;
    }
    glutPostRedisplay();
}

static void special2(int key, int, int) {
    const float step = 3.0f;
    if (key == GLUT_KEY_LEFT)  camYaw  -= step;
    if (key == GLUT_KEY_RIGHT) camYaw  += step;
    if (key == GLUT_KEY_UP)    camPitch= std::min(85.0f, camPitch + step);
    if (key == GLUT_KEY_DOWN)  camPitch= std::max(-85.0f, camPitch - step);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(g_width2, g_height2);
    glutCreateWindow("Enhanced Material Teapot - GodLiangJingYu");

    setupLightsAndMaterial2();

    glutDisplayFunc(display2);
    glutReshapeFunc(reshape2);
    glutKeyboardFunc(keyboard2);
    glutSpecialFunc(special2);
    glutIdleFunc(idle2);

    printf("\n=== Enhanced Material Teapot ===\n");
    printf("Material: Purple diffuse(0.8), White ambient(0.2)\n");
    printf("          Magenta specular(0.5), Shininess(50), Emission(0.5)\n");
    printf("Press H for help\n\n");

    glutMainLoop();
    return 0;
}