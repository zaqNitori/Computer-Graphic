//
//#include <stdio.h> 
//#include <stdlib.h>
//#include <time.h>
//#include <set>
//#include <cmath>
//#include <vector>
//using namespace std;
//
///*** freeglut***/
//#include ".\GL\freeglut.h"
//
//#define _USE_MATH_DEFINES
//#define mkpr(x, y) make_pair(x, y)
//#define fi first
//#define se second
//#define COS(angle) cos((angle) * M_PI / 180)
//#define SIN(angle) sin((angle) * M_PI / 180)
//
//enum class RasLine_Algorithm
//{
//    MidPoint = 1,
//    Anti_Alias = 2
//};
//
//typedef pair<int, int> pii;
//typedef pair<float, float> pff;
//
//vector<pii> lineVector;
//int menuSize, range;
//RasLine_Algorithm usingAlgorithm;
//GLfloat window_width, window_height;
//GLfloat orthoL, orthoR, orthoT, orthoB;
//
//void ChangeSize(int, int);
//void RenderScene();
//void mySpecialKey(int, int, int);
//void myKeyboard(unsigned char, int, int);
//void myMouse(int, int, int, int);
//void myMenu(int);
//void algorithmMenu(int);
//void sizeMenu(int);
//
//#pragma region Init
//
//void InitialParameter()
//{
//    window_height = window_width = 600;
//    menuSize = 10;
//    orthoL = orthoB = -10;
//    orthoR = orthoT = 10;
//    range = window_width / 2 / menuSize;    //dot for each grid
//    usingAlgorithm = RasLine_Algorithm::MidPoint;                     // Default MidPoint
//}
//
//void InitialMenu()
//{
//    int algMenu = glutCreateMenu(algorithmMenu);
//    glutAddMenuEntry("MidPoint", 1);
//    glutAddMenuEntry("Anti-Aliasing", 2);
//
//    int msMenu = glutCreateMenu(sizeMenu);
//    glutAddMenuEntry("10", 10);
//    glutAddMenuEntry("15", 15);
//    glutAddMenuEntry("20", 20);
//    glutAddMenuEntry("25", 25);
//
//    glutCreateMenu(myMenu);
//    glutAddSubMenu("Map Size", msMenu);
//    glutAddSubMenu("Algorithm", algMenu);
//    glutAttachMenu(GLUT_RIGHT_BUTTON);
//}
//
//void InitialGlut(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowSize(window_width, window_height);
//    glutInitWindowPosition(600, 80);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glutCreateWindow("Rasterization Line");
//}
//#pragma endregion
//
//int main(int argc, char** argv)
//{
//    srand(time(NULL));
//    InitialParameter();
//    InitialGlut(argc, argv);
//    InitialMenu();
//
//    glutKeyboardFunc(myKeyboard);
//    glutMouseFunc(myMouse);
//    glutSpecialFunc(mySpecialKey);
//    glutDisplayFunc(RenderScene);
//    glutReshapeFunc(ChangeSize);
//    glutMainLoop();
//
//    return 0;
//}
//
//#pragma region View Method
//
//void ChangeSize(int w, int h)
//{
//    printf("Window Size= %d X %d\n", w, h);
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(orthoL, orthoR, orthoB, orthoT, -10, 10);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}
//
//void SetOrtho(int size)
//{
//    orthoB = orthoL = -size;
//    orthoR = orthoT = size;
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(orthoL, orthoR, orthoB, orthoT, -10, 10);
//}
//
//#pragma endregion
//
//
//#pragma region Draw Method
//
//void DrawMesh()
//{
//    glColor3f(0, 0, 0);
//    glPointSize(4.0f);
//    glBegin(GL_POINTS);
//    glVertex3f(0, 0, 0);
//    glEnd();
//
//    //繪製表格
//    for (float i = -menuSize; i <= menuSize; i++)
//    {
//        glColor3f(0, 0, 0);
//        glBegin(GL_LINES);
//        glVertex3f(i, menuSize, 0);
//        glVertex3f(i, -menuSize, 0);
//        glEnd();
//
//        glColor3f(0, 0, 0);
//        glBegin(GL_LINES);
//        glVertex3f(menuSize, i, 0);
//        glVertex3f(-menuSize, i, 0);
//        glEnd();
//    }
//
//}
//
//void FillGrid(int fx, int fy)
//{
//
//    glBegin(GL_QUADS);
//    glColor3f(0, 0, 0);
//    glVertex3f(fx, fy, 0);
//    glVertex3f(fx + 1, fy, 0);
//    glVertex3f(fx + 1, fy + 1, 0);
//    glVertex3f(fx, fy + 1, 0);
//    glEnd();
//
//}
//
//void FillGrid(int fx, int fy, float err)
//{
//    glBegin(GL_QUADS);
//    glColor3f(err , err, err);
//    glVertex3f(fx, fy, 0);
//    glVertex3f(fx + 1, fy, 0);
//    glVertex3f(fx + 1, fy + 1, 0);
//    glVertex3f(fx, fy + 1, 0);
//    glEnd();
//}
//
//void MidPoint(int x0, int y0, int x1, int y1)
//{
//    int bx, by;
//    bx = by = 1;
//    float m = (x1 - x0) ? (float)(y1 - y0) / (x1 - x0) : 99;
//
//    //to change the line direction into left to right / bottom to top
//    if ((x0 > x1 && abs(m) <= 1) || (y0 > y1 && abs(m) > 1))
//    {
//        swap(x0, x1);
//        swap(y0, y1);
//    }
//
//    int dx = x1 - x0;
//    int dy = y1 - y0;
//
//    if (dx < 0)
//    {
//        dx = -dx;
//        bx ^= 1;
//    }
//
//    if (dy < 0)
//    {
//        dy = -dy;
//        by ^= 1;
//    }
//
//    // to determine x / y which should be mainly span
//    int d = (abs(m) <= 1) ? 2 * dy - dx : 2 * dx - dy;
//    int delE = (abs(m) <= 1) ? 2 * dy : 2 * dx;
//    int delNE = (abs(m) <= 1) ? 2 * (dy - dx) : 2 * (dx - dy);
//
//    int x = x0, y = y0;
//    FillGrid(x, y);
//    printf("%d, %d\n", x, y);
//
//    if (abs(m) <= 1)
//    {
//        while (x < x1)
//        {
//            if (d <= 0)
//            {
//                d += delE;
//                x++;
//            }
//            else
//            {
//                d += delNE;
//                x++;
//                by ? y++ : y--;
//            }
//            FillGrid(x, y);
//            printf("%d, %d\n", x, y);
//        }
//    }
//    else
//    {
//        while (y < y1)
//        {
//            if (d <= 0)
//            {
//                d += delE;
//                y++;
//            }
//            else
//            {
//                d += delNE;
//                y++;
//                bx ? x++ : x--;
//            }
//            FillGrid(x, y);
//            printf("%d, %d\n", x, y);
//        }
//    }
//}
//
//void Anti_Aliasing(int x0, int y0, int x1, int y1)
//{
//    float m = (x1 - x0) ? (float)(y1 - y0) / (x1 - x0) : 1000;
//
//    if ((x0 > x1 && abs(m) <= 1) || (y0 > y1 && abs(m) > 1))
//    {
//        swap(x0, x1);
//        swap(y0, y1);
//    }
//
//    int dx = x1 - x0;
//    int dy = y1 - y0;
//    float x = x0;
//    float y = y0;
//
//    if (abs(m) <= 1)    //run x
//    {
//        int ty;
//        for (; x <= x1; x++)
//        {
//            ty = floor(y);
//            float err = y - ty;
//            FillGrid(x, y, err);
//            FillGrid(x, y + 1, 1 - err);
//            y += m;
//        }
//    }
//    else                //run y
//    {
//        m = dy ? (float)dx / dy : 0;
//        int tx;
//        for (; y <= y1; y++)
//        {
//            tx = floor(x);
//            float err = x - tx;
//            FillGrid(x, y, err);
//            FillGrid(x + 1, y, 1 - err);
//            x += m;
//        }
//    }
//
//}
//
//void FillLine(int x0, int y0, int x1, int y1)
//{
//    switch (usingAlgorithm)
//    {
//    case RasLine_Algorithm::MidPoint:
//        MidPoint(x0, y0, x1, y1);
//        break;
//    case RasLine_Algorithm::Anti_Alias:
//        Anti_Aliasing(x0, y0, x1, y1);
//        break;
//    default:
//        break;
//    }
//}
//
//void DrawLine()
//{
//    
//    int limit = lineVector.size() / 2;
//    float fx1, fx2, fy1, fy2;
//    for (int i = 0; i < limit; i++)
//    {
//        fx1 = (float)lineVector[i * 2].fi / range;
//        fy1 = (float)lineVector[i * 2].se / range;
//        fx2 = (float)lineVector[i * 2 + 1].fi / range;
//        fy2 = (float)lineVector[i * 2 + 1].se / range;
//        FillLine(floor(fx1), floor(fy1), floor(fx2), floor(fy2));
//    }
//}
//
//void DrawPoint()
//{
//    int len = lineVector.size();
//    if (!len) return;
//    if (len & 1)
//    {
//        glColor3f(0, 1, 0);
//        glPointSize(4.0f);
//        glBegin(GL_POINTS);
//        glVertex3f((float)lineVector.back().fi / range, (float)lineVector.back().se / range, 0);
//        glEnd();
//    }
//    else
//    {
//        glColor3f(0, 1, 0);
//        glPointSize(4.0f);
//        glBegin(GL_POINTS);
//        glVertex3f((float)lineVector[len - 2].fi / range, (float)lineVector[len - 2].se / range, 0);
//        glEnd();
//
//        glColor3f(1, 0, 0);
//        glPointSize(4.0f);
//        glBegin(GL_POINTS);
//        glVertex3f((float)lineVector.back().fi / range, (float)lineVector.back().se / range, 0);
//        glEnd();
//    }
//}
//
//void RenderScene()
//{
//    glClearColor(1.0, 1.0, 1.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0, 0, 5.0f
//        , 0, 0, 0
//        , 0, 1, 0);
//
//    
//    DrawLine();
//    DrawMesh();
//    DrawPoint();
//
//    glutSwapBuffers();
//}
//
//#pragma endregion
//
//
//#pragma region Input Method
//
//void myKeyboard(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//    case 'r':
//        lineVector.clear();
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();
//}
//
//void myMouse(int button, int state, int x, int y)
//{
//    GLfloat nx, ny;
//    int mx, my;
//
//    if (!menuSize) return;
//    switch (button)
//    {
//    case GLUT_LEFT_BUTTON:
//        if (state == GLUT_DOWN)
//        {
//            //座標轉換，原點在中心
//            nx = x - window_width / 2;
//            ny = window_height / 2 - y;
//            //printf("%.2f, %.2f\n", nx, ny);
//
//            pii tp = mkpr(nx, ny);
//            lineVector.push_back(tp);
//
//            /*mx = (nx >= 0 ? (nx + range - 1) : (nx - range + 1)) / range;
//            my = (ny >= 0 ? (ny + range - 1) : (ny - range + 1)) / range;
//            printf("%d, %d\n\n", mx, my);*/
//
//        }
//        break;
//    default:
//        break;
//    }
//
//    glutPostRedisplay();
//}
//
//void mySpecialKey(int key, int x, int y)
//{
//    glutPostRedisplay();
//}
//
//#pragma endregion
//
//
//#pragma region Menu
//void myMenu(int index)
//{
//    //
//}
//
//void algorithmMenu(int index)
//{
//    switch (index)
//    {
//    case 1:         //midpoint
//        usingAlgorithm = RasLine_Algorithm::MidPoint;
//        break;
//    case 2:         //anti-aliasing
//        usingAlgorithm = RasLine_Algorithm::Anti_Alias;
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();
//}
//
//void sizeMenu(int index)
//{
//    switch (index)
//    {
//    case 10:
//        menuSize = 10;
//        break;
//    case 15:
//        menuSize = 15;
//        break;
//    case 20:
//        menuSize = 20;
//        break;
//    case 25:
//        menuSize = 25;
//        break;
//    default:
//        menuSize = 0;
//        break;
//    }
//    SetOrtho(menuSize);
//    range = window_width / 2 / menuSize;
//    glutPostRedisplay();
//}
//#pragma endregion
