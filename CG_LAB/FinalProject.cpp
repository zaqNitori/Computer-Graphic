
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

typedef struct vertex
{
    float r, g, b;
    float x, y, z;
    vertex() :x{ 0 }, y{ 0 }, z{ 0 }, r{ 0 }, g{ 0 }, b{ 0 } {}
    vertex(float fx, float fy, float fz, float fr, float fg, float fb) :
        x{ fx }, y{ fy }, z{ fz }, r{ fr }, g{ fg }, b{ fb }{}

}Vertex;

typedef struct face
{
    int p1, p2, p3;
    face(int p1, int p2, int p3) :
        p1{ p1 }, p2{ p2 }, p3{ p3 }{}

}Face;

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
vector<Face> faceVector;
FileMode flMode;
RenderMode rdMode;
ColorMode crMode;
BoundingMode bdMode;

GLfloat rotateX, rotateY;
GLfloat translateX, translateY;
GLfloat window_width, window_height;


string filePath[] = {
    "None",
    "./material/final/gourd.obj",
    "./material/final/teapot.obj",
    "./material/final/teddy.obj",
    "./material/final/octahedron.obj"
};

void ObjParser();
void ChangeSize(int, int);
void RenderScene();
void mySpecialKey(int, int, int);
void myKeyboard(unsigned char, int, int);
void myMouse(int, int, int, int);
void InitialMenu();

void ObjParser()
{
    ifstream ifs;
    string line;
    float f[3];
    int p[3];
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
        ifs.close();
        return;
    }

    vertexVector.clear();
    faceVector.clear();

    while (ifs >> line)
    {
    	if (line == "f")
    	{
    		idx = 0;
    		while (ifs >> p[idx++] && idx < 3);

            Face f(p[0] - 1, p[1] - 1, p[2] - 1);
            faceVector.push_back(f);
    	}
        else if (line == "v")
        {
            idx = 0;
            while (ifs >> f[idx++] && idx < 3);

            double r, g, b;
            r = ((double)rand() / (RAND_MAX));
            g = ((double)rand() / (RAND_MAX));
            b = ((double)rand() / (RAND_MAX));
            Vertex v(f[0], f[1], f[2], r, g, b);
            vertexVector.push_back(v);
        }
    }
    ifs.close();

    return;
}

void InitialParameter()
{
    window_height = 600;
    window_width = 800;
    vertexVector.clear();
    faceVector.clear();
    flMode = FileMode::octahedron;
    rdMode = RenderMode::Line;
    crMode = ColorMode::Single;
    bdMode = BoundingMode::On;

    translateX = translateY = 0;
    rotateX = rotateY = 0;
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

int main(int argc, char** argv)
{
    srand(time(NULL));
    InitialParameter();
    InitialGlut(argc, argv);
    InitialMenu();

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
    int ortho = 5;
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

void DrawObj()
{
    glPushMatrix();

    glTranslatef(translateX, translateY, 0);
    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);

    Vertex nv;
    for (auto au : faceVector)
    {
        glBegin(GL_TRIANGLES);
        nv = vertexVector[au.p1];
        glColor3f(nv.r, nv.g, nv.b);
        glVertex3f(nv.x, nv.y, nv.z);

        nv = vertexVector[au.p2];
        glPointSize(4.0f);
        glColor3f(nv.r, nv.g, nv.b);
        glVertex3f(nv.x, nv.y, nv.z);

        nv = vertexVector[au.p3];
        glPointSize(4.0f);
        glColor3f(nv.r, nv.g, nv.b);
        glVertex3f(nv.x, nv.y, nv.z);
        glEnd();
    }

    glPopMatrix();
}

void SetDrawAttribute()
{
    switch (rdMode)
    {
    case Point:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case Line:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case Fill:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    default:
        break;
    }

    switch (crMode)
    {
    case Single:
        glShadeModel(GL_FLAT);
        break;
    case Random:
        glShadeModel(GL_SMOOTH);
        break;
    default:
        break;
    }
}

void RenderScene()
{
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5
        , 0, 0, 0
        , 0, 1, 0);

    
    SetDrawAttribute();
    DrawObj();


    glutSwapBuffers();
}

#pragma endregion


#pragma region Input Method

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        translateX = translateY = 0;
        rotateX = rotateY = 0;
        break;
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
        rotateY -= shift;
        break;
    case GLUT_KEY_RIGHT:
        rotateY += shift;
        break;
    case GLUT_KEY_UP:
        rotateX -= shift;
        break;
    case GLUT_KEY_DOWN:
        rotateX += shift;
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
    case ColorMode::Single:
        crMode = ColorMode::Single;
        break;
    case ColorMode::Random:
        crMode = ColorMode::Random;
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
