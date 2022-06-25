
#include <stdio.h> 
#include <stdlib.h>
#include <fstream>
#include <string>
#include <time.h>
#include <set>
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

/*** freeglut***/
#include ".\GL\freeglut.h"

#define _USE_MATH_DEFINES
#define mkpr(x, y) make_pair(x, y)
#define fi first
#define se second
#define COS(angle) cos((angle) * M_PI / 180)
#define SIN(angle) sin((angle) * M_PI / 180)

typedef pair<int, int> pii;
typedef pair<float, float> pff;

typedef struct
{
    float r, g, b;
    float x, y, z;
}Vertex;

#pragma region Enum

enum FileMode {
    ground = 1,
    teapot = 2,
    teddy = 3,
    octahedron = 4,
    Input = 5
};

enum RenderMode {
    Point = 1,
    Line = 2,
    Fill = 3
};

enum ColorMode {
    Single = 1,
    Random = 2
};

enum BoundingMode {
    On = 1,
    Off = 2
};

#pragma endregion

vector<Vertex> vertexVector;
FileMode flMode;
RenderMode rdMode;
ColorMode crMode;
BoundingMode bdMode;
GLfloat window_width, window_height;
string filePath[] = {
    "None",
    "./material/final/gourd.obj",
    "./material/final/teapot.obj",
    "./material/final/teddy.obj",
    "./material/final/octahedron.obj"
};

bool ObjParser();
void ChangeSize(int, int);
void RenderScene();
void mySpecialKey(int, int, int);
void myKeyboard(unsigned char, int, int);
void myMouse(int, int, int, int);
void InitialMenu();

bool ObjParser()
{
    ifstream ifs;
    string line;
    float f[3];
    int idx;
    
    if (flMode == FileMode::Input)
    {
        char buf[100];
        cin.getline(buf, 100);
        ifs.open(buf, ifstream::in);
    }
    else
        ifs.open(filePath[flMode], ifstream::in);

    if (!ifs.good())
    {
        printf("Failed to open file!\n");
        return false;
    }

    int t = 0;
    while (ifs >> line)
    {
    	if (line == "f" || line == "v")
    	{
            t++;
    		idx = 0;
    		while (ifs >> f[idx++] && idx < 3);
            printf("%f, %f, %f\n", f[0],f[1],f[2]);
    	}
        if (t > 10) break;
    }
    ifs.close();

    return true;
}

void InitialParameter()
{
    window_height = 600;
    window_width = 800;
    vertexVector.clear();
    flMode = FileMode::octahedron;
    rdMode = RenderMode::Line;
    crMode = ColorMode::Single;
    bdMode = BoundingMode::On;
}

void InitialGlut(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(500, 80);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutCreateWindow("Final_Project");
}

void LightSetting()
{
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void MaterialSetting()
{
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
    GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
    GLfloat mat_shininess = { 20.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    //glShadeModel(GL_SMOOTH); /*enable smooth shading */
    glEnable(GL_LIGHTING); /* enable lighting */
    glEnable(GL_LIGHT0); /* enable light 0 */
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    InitialParameter();
    InitialGlut(argc, argv);
    InitialMenu();
    LightSetting();
    MaterialSetting();

    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    glutSpecialFunc(mySpecialKey);
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutMainLoop();

    return 0;
}

#pragma region View Method

void ChangeSize(int w, int h)
{
    int ortho = 10;
    printf("Window Size= %d X %d\n", w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ortho, ortho, -ortho, ortho, -ortho, ortho);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

#pragma endregion

#pragma region Draw Method

void RenderScene()
{
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 3.0f
        , 0, 0, 0
        , 0, 1, 0);

    glutSwapBuffers();
}

#pragma endregion


#pragma region Input Method

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    default:
        break;
    }
    glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{

    switch (button)
    {

    default:
        break;
    }

    glutPostRedisplay();
}

void mySpecialKey(int key, int x, int y)
{
    float shift = 4.0f;
    switch (key)
    {
    case GLUT_KEY_LEFT:
        break;
    case GLUT_KEY_RIGHT:
        break;
    case GLUT_KEY_UP:
        break;
    case GLUT_KEY_DOWN:
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

#pragma endregion


#pragma region Menu

void myMenu(int index)
{
    //
}

void fileMenu(int index)
{
    switch (index)
    {
    case FileMode::ground:
        flMode = FileMode::ground;
        break;
    case FileMode::teapot:
        flMode = FileMode::teapot;
        break;
    case FileMode::teddy:
        flMode = FileMode::teddy;
        break;
    case FileMode::octahedron:
        flMode = FileMode::octahedron;
        break;
    case FileMode::Input:
        flMode = FileMode::Input;
        break;
    default:
        break;
    }

    ObjParser();
    glutPostRedisplay();
}

void modeMenu(int index)
{
    switch (index)
    {
    case RenderMode::Point:
        rdMode = RenderMode::Point;
        break;
    case RenderMode::Line:
        rdMode = RenderMode::Line;
        break;
    case RenderMode::Fill:
        rdMode = RenderMode::Fill;
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void colorMenu(int index)
{
    switch (index)
    {
    case RenderMode::Point:
        rdMode = RenderMode::Point;
        break;
    case RenderMode::Line:
        rdMode = RenderMode::Line;
        break;
    case RenderMode::Fill:
        rdMode = RenderMode::Fill;
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void boundingMenu(int index)
{
    switch (index)
    {
    case BoundingMode::On:
        bdMode = BoundingMode::On;
        break;
    case BoundingMode::Off:
        bdMode = BoundingMode::Off;
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void InitialMenu()
{
#pragma region FileMenu
    int fmenu = glutCreateMenu(fileMenu);
    glutAddMenuEntry("ground", FileMode::ground);
    glutAddMenuEntry("teapot", FileMode::teapot);
    glutAddMenuEntry("teddy", FileMode::teddy);
    glutAddMenuEntry("octahedron", FileMode::octahedron);
    glutAddMenuEntry("input", FileMode::Input);
#pragma endregion

#pragma region RenderMenu
    int rmenu = glutCreateMenu(modeMenu);
    glutAddMenuEntry("Point", RenderMode::Point);
    glutAddMenuEntry("Line", RenderMode::Line);
    glutAddMenuEntry("Fill", RenderMode::Fill);
#pragma endregion

#pragma region ColorMenu
    int cmenu = glutCreateMenu(colorMenu);
    glutAddMenuEntry("Single", ColorMode::Single);
    glutAddMenuEntry("Random", ColorMode::Random);
#pragma endregion

#pragma region BoundingBox
    int bmenu = glutCreateMenu(boundingMenu);
    glutAddMenuEntry("On", BoundingMode::On);
    glutAddMenuEntry("Off", BoundingMode::Off);
#pragma endregion

    glutCreateMenu(myMenu);
    glutAddSubMenu("File Mode", fmenu);
    glutAddSubMenu("Polygon Mode", rmenu);
    glutAddSubMenu("Color Mode", cmenu);
    glutAddSubMenu("Bounding Mode", bmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#pragma endregion
