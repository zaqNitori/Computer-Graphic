
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
GLfloat scaling;
GLfloat window_width, window_height;
GLfloat maxX, minX, maxY, minY, maxZ, minZ;

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

void FindBoundingBox(float fp[3])
{
    maxX = max(maxX, fp[0]);
    maxY = max(maxY, fp[1]);
    maxZ = max(maxZ, fp[2]);

    minX = min(minX, fp[0]);
    minY = min(minY, fp[1]);
    minZ = min(minZ, fp[2]);
}

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
        ifs.open("./material/final/" + string(buf), ifstream::in);
    }
    else
        ifs.open(filePath[flMode], ifstream::in);

    if (!ifs.good())
    {
        printf("Failed to open file!\n");
        ifs.close();
        return;
    }

    maxX = maxY = maxZ = INT_MIN;
    minX = minY = minZ = INT_MAX;
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

            FindBoundingBox(f);

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
    bdMode = BoundingMode::Off;

    translateX = translateY = 0;
    rotateX = rotateY = 0;
    scaling = 1;

    maxX = maxY = maxZ = INT_MIN;
    minX = minY = minZ = INT_MAX;
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
}

void SetDrawAttribute()
{
    switch (rdMode)
    {
    case Point:
        glPointSize(4.0f);
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

void DrawBoundingBox()
{
    if (bdMode == BoundingMode::Off) return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1, 1, 1);
    // left
    glBegin(GL_QUADS);
    glVertex3f(minX, minY, minZ);
    glVertex3f(minX, minY, maxZ);
    glVertex3f(minX, maxY, maxZ);
    glVertex3f(minX, maxY, minZ);
    glEnd();

    // right
    glBegin(GL_QUADS);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(maxX, minY, maxZ);
    glVertex3f(maxX, maxY, maxZ);
    glVertex3f(maxX, maxY, minZ);
    glEnd();

    // front
    glBegin(GL_QUADS);
    glVertex3f(minX, minY, maxZ);
    glVertex3f(maxX, minY, maxZ);
    glVertex3f(maxX, maxY, maxZ);
    glVertex3f(minX, maxY, maxZ);
    glEnd();

    // back
    glBegin(GL_QUADS);
    glVertex3f(minX, minY, minZ);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(maxX, maxY, minZ);
    glVertex3f(minX, maxY, minZ);
    glEnd();
}

void SetOrtho()
{
    float scale = 2;
    GLfloat small, big;

    small = (minX > minY) ? (minY > minZ) ? minZ : minY : (minX > minZ) ? minZ : minX;
    big = (maxX > maxY) ? (maxX > maxZ) ? maxX : maxZ : (maxY > maxZ) ? maxY : maxZ;
    printf("%f, %f, %f, %f,%f, %f\n", minX, maxX, minY, maxY, minZ, maxZ);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(small * scale, big * scale, small * scale, big * scale, small * scale, big * scale);
}

void RenderScene()
{
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    SetOrtho();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, .5
        , 0, 0, 0
        , 0, 1, 0);

    
    SetDrawAttribute();

    glPushMatrix();
    {
        glTranslatef(translateX, translateY, 0);
        glRotatef(rotateX, 1, 0, 0);
        glRotatef(rotateY, 0, 1, 0);
        glRotatef(0, 0, 0, 1);
        glScalef(scaling, scaling, scaling);

        glPushMatrix();
        DrawObj();
        glPopMatrix();

        DrawBoundingBox();
    }
    glPopMatrix();

    glutSwapBuffers();
}

#pragma endregion


#pragma region Input Method

void myKeyboard(unsigned char key, int x, int y)
{
    float shift = 4.0f;
    switch (key)
    {
    case 'r':
        translateX = translateY = 0;
        rotateX = rotateY = 0;
        scaling = 1;
        break;
    case 'a':
        rotateY -= shift;
        break;
    case 'd':
        rotateY += shift;
        break;
    case 'w':
        rotateX -= shift;
        break;
    case 's':
        rotateX += shift;
        break;
    case '+':
        if(scaling < 5)
            scaling += 0.5;
        break;
    case '-':
        if (scaling > 0.5)
            scaling -= 0.5;
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
        translateX -= shift;
        break;
    case GLUT_KEY_RIGHT:
        translateX += shift;
        break;
    case GLUT_KEY_UP:
        translateY += shift;
        break;
    case GLUT_KEY_DOWN:
        translateY -= shift;
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
    scaling = 1;
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
        scaling = 2;
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
    glutAddSubMenu("Render Mode", rmenu);
    glutAddSubMenu("Color Mode", cmenu);
    glutAddSubMenu("Bounding Mode", bmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#pragma endregion
