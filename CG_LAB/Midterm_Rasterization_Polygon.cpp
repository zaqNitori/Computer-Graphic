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
//enum class Rasterize_Option
//{
//    Line = 1,
//    Polygon = 2
//};
//
//typedef struct
//{
//    float r, g, b;
//    float x, y;
//}Vertex;
//
//typedef pair<int, int> pii;
//typedef pair<float, float> pff;
//
//bool stop_polygon_input;
//int menuSize, range;
//vector<Vertex> mouseDownVertex;
//vector<Vertex> transformVertex;
//RasLine_Algorithm usingAlgorithm;
//Rasterize_Option rasOption;
//GLfloat window_width, window_height;
//GLfloat orthoL, orthoR, orthoT, orthoB;
//
//void ChangeSize(int, int);
//void RenderScene();
//void mySpecialKey(int, int, int);
//void myKeyboard(unsigned char, int, int);
//void myMouse(int, int, int, int);
//void InitialMenu();
//
//void InitialParameter()
//{
//    window_height = window_width = 600;
//    menuSize = 10;
//    orthoL = orthoB = -10;
//    orthoR = orthoT = 10;
//    range = window_width / 2 / menuSize;    //dot for each grid
//    usingAlgorithm = RasLine_Algorithm::MidPoint;           // Default MidPoint
//    rasOption = Rasterize_Option::Line;                     // Default Line
//    stop_polygon_input = false;
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
//void FillGrid(int fx, int fy, float r, float g, float b)
//{
//    glBegin(GL_QUADS);
//    glColor3f(r, g, b);
//    glVertex3f(fx, fy, 0);
//    glVertex3f(fx + 1, fy, 0);
//    glVertex3f(fx + 1, fy + 1, 0);
//    glVertex3f(fx, fy + 1, 0);
//    glEnd();
//}
//
//#pragma region Rasterize_Line
//
//void MidPoint(Vertex v1, Vertex v2)
//{
//    int x0 = floor(v1.x);
//    int y0 = floor(v1.y);
//    int x1 = floor(v2.x);
//    int y1 = floor(v2.y);
//    float r = v1.r;
//    float g = v1.g;
//    float b = v1.b;
//
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
//    float dr = v2.r - v1.r;
//    dr = (abs(m) <= 1) ? dr / dx : dr / dy;
//    float dg = v2.g - v1.g;
//    dg = (abs(m) <= 1) ? dg / dx : dg / dy;
//    float db = v2.b - v1.b;
//    db = (abs(m) <= 1) ? db / dx : db / dy;
//
//    int x = x0, y = y0;
//    FillGrid(x, y, r, g, b);
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
//            r += dr;
//            g += dg;
//            b += db;
//            FillGrid(x, y, r, g, b);
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
//            r += dr;
//            g += dg;
//            b += db;
//            FillGrid(x, y, r, g, b);
//            printf("%d, %d\n", x, y);
//        }
//    }
//}
//
//void Anti_Aliasing(Vertex v1, Vertex v2)
//{
//    int x0 = floor(v1.x);
//    int y0 = floor(v1.y);
//    int x1 = floor(v2.x);
//    int y1 = floor(v2.y);
//
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
//void FillLine(Vertex v1, Vertex v2)
//{
//    switch (usingAlgorithm)
//    {
//    case RasLine_Algorithm::MidPoint:
//        MidPoint(v1, v2);
//        break;
//    case RasLine_Algorithm::Anti_Alias:
//        Anti_Aliasing(v1, v2);
//        break;
//    default:
//        break;
//    }
//}
//
//void Rasterize_Line()
//{
//
//    int len = transformVertex.size();
//    float fx1, fx2, fy1, fy2;
//    for (int i = 0; i < len - 1; i++)
//    {
//        FillLine(transformVertex[i], transformVertex[i + 1]);
//    }
//    if (len && rasOption == Rasterize_Option::Polygon)
//        FillLine(transformVertex[0], transformVertex[len - 1]);
//
//}
//
//#pragma endregion
//
//#pragma region Rasterize_Polygon
//
//void diff(Vertex *oldVer, Vertex *newVer, Vertex *nowVer, Vertex *mvVer, float dis, float errY)
//{
//    mvVer->x = (newVer->x - oldVer->x) / dis;
//    nowVer->x = oldVer->x + errY * mvVer->x;
//
//    mvVer->r = (newVer->r - oldVer->r) / dis;
//    nowVer->r = oldVer->r + errY * mvVer->r;
//
//    mvVer->g = (newVer->g - oldVer->g) / dis;
//    nowVer->g = oldVer->g + errY * mvVer->g;
//
//    mvVer->b = (newVer->b - oldVer->b) / dis;
//    nowVer->b = oldVer->b + errY * mvVer->b;
//}
//
//void diffY(Vertex *oldVer, Vertex *newVer, Vertex *nowVer, Vertex *mvVer, float nowy)
//{
//    float dis = (newVer->y - oldVer->y);
//    float err = (nowy - oldVer->y);
//    diff(oldVer, newVer, nowVer, mvVer, dis, err);
//}
//
//void diffX(Vertex* leftVer, Vertex* rightVer, Vertex* nowVer, Vertex* mvVer, float nowx)
//{
//    float dis = (rightVer->x - leftVer->x);
//    float err = (nowx - leftVer->x);
//    diff(leftVer, rightVer, nowVer, mvVer, dis, err);
//}
//
//void moveVertex(Vertex* ver, Vertex* mv)
//{
//    ver->x += mv->x;
//    ver->r += mv->r;
//    ver->g += mv->g;
//    ver->b += mv->b;
//}
//
//void scanX(Vertex vl, Vertex vr, int y)
//{
//    int nowx, lx, rx;
//    Vertex ver, mv;
//
//    nowx = lx = ceil(vl.x);
//    rx = ceil(vr.x);
//
//    if (lx < rx)
//    {
//        diffX(&vl, &vr, &ver, &mv, nowx);
//        for (; nowx < rx; nowx++)
//        {
//            FillGrid(nowx, y, ver.r, ver.g, ver.b);
//            moveVertex(&ver, &mv);
//        }
//    }
//
//}
//
//void scanY(int startIndex)
//{
//    int lIdx, rIdx, nowIdx;
//    int ly, ry, nowy;
//    Vertex verl, mvl;
//    Vertex verr, mvr;
//    int remain, len;
//
//    lIdx = rIdx = nowIdx = startIndex;
//    ly = ry = nowy = ceil(transformVertex[startIndex].y);
//
//    len = remain = transformVertex.size();
//
//    while (remain)
//    {
//        while (remain && ly <= nowy)
//        {
//            remain--;
//
//            nowIdx = lIdx - 1;
//            if (nowIdx < 0)
//                nowIdx = len - 1;
//
//            ly = ceil(transformVertex[nowIdx].y);
//
//            if (ly > nowy)
//                diffY(&transformVertex[lIdx], &transformVertex[nowIdx], &verl, &mvl, nowy);
//
//            lIdx = nowIdx;
//        }
//
//        while (remain && ry <= nowy)
//        {
//            remain--;
//
//            nowIdx = rIdx + 1;
//            if (nowIdx >= len)
//                nowIdx = 0;
//
//            ry = ceil(transformVertex[nowIdx].y);
//
//            if (ry > nowy)
//                diffY(&transformVertex[rIdx], &transformVertex[nowIdx], &verr, &mvr, nowy);
//
//            rIdx = nowIdx;
//        }
//
//        for (; nowy < ly && nowy < ry; nowy++)
//        {
//            scanX(verl, verr, nowy);
//            moveVertex(&verl, &mvl);
//            moveVertex(&verr, &mvr);
//        }
//
//    }
//
//}
//
//void Crow_Algorithm(int startIndex)
//{
//    scanY(startIndex);
//}
//
//void Rasterize_Polygon()
//{
//    if (!stop_polygon_input) return;
//
//    int len = transformVertex.size();
//    int yMin = 0;
//
//    for (int i = 1; i < len; i++)
//    {
//        if (transformVertex[i].y < transformVertex[yMin].y)
//            yMin = i;
//    }
//
//    Crow_Algorithm(yMin);
//}
//
//#pragma endregion
//
//void Transform()
//{
//    Vertex v;
//    transformVertex.clear();
//    for (auto au : mouseDownVertex)
//    {
//        v = au;
//        v.x /= range;
//        v.y /= range;
//        transformVertex.push_back(v);
//    }
//}
//
//void DrawHint()
//{
//    int len = transformVertex.size();
//    float pointSize = 7.0f;
//    if (!len) return;
//    
//    float fx1, fx2, fy1, fy2;
//    for (int i = 0; i < len - 1; i++)
//    {
//        fx1 = transformVertex[i].x;
//        fx2 = transformVertex[i+1].x;
//        fy1 = transformVertex[i].y;
//        fy2 = transformVertex[i+1].y;
//        glColor3f(0, 0, 0);
//        glBegin(GL_LINES);
//        glVertex3f(fx1, fy1, 0);
//        glVertex3f(fx2, fy2, 0);
//        glEnd();
//
//        glColor3f(transformVertex[i].r, transformVertex[i].g, transformVertex[i].b);
//        glPointSize(pointSize);
//        glBegin(GL_POINTS);
//        glVertex3f(fx1, fy1, 0);
//        glEnd();
//
//        glColor3f(transformVertex[i + 1].r, transformVertex[i + 1].g, transformVertex[i + 1].b);
//        glPointSize(pointSize);
//        glBegin(GL_POINTS);
//        glVertex3f(fx2, fy2, 0);
//        glEnd();
//
//    }
//
//    if (rasOption == Rasterize_Option::Polygon)
//    {
//        glColor3f(0, 0, 0);
//        glBegin(GL_LINES);
//        glVertex3f(transformVertex[0].x, transformVertex[0].y, 0);
//        glVertex3f(transformVertex[len - 1].x, transformVertex[len - 1].y, 0);
//        glEnd();
//    }
//
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
//    Transform();
//
//    Rasterize_Line();
//    if (rasOption == Rasterize_Option::Polygon)
//        Rasterize_Polygon();
//
//    DrawMesh();
//    DrawHint();
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
//        stop_polygon_input = false;
//        mouseDownVertex.clear();
//        break;
//    case 's':
//        if (mouseDownVertex.size() > 2 && rasOption == Rasterize_Option::Polygon)
//            stop_polygon_input = true;
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
//
//            if (!stop_polygon_input)
//            {
//                Vertex v;
//                v.x = nx;
//                v.y = ny;
//                v.r = ((double)rand() / (RAND_MAX));
//                v.g = ((double)rand() / (RAND_MAX));
//                v.b = ((double)rand() / (RAND_MAX));
//                mouseDownVertex.push_back(v);
//            }
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
//
//
//void myMenu(int index)
//{
//    //
//}
//
//void RasterizeMenu(int index)
//{
//    switch (index)
//    {
//    case 1:
//        rasOption = Rasterize_Option::Line;
//        break;
//    case 2:
//        rasOption = Rasterize_Option::Polygon;
//        stop_polygon_input = false;
//        mouseDownVertex.clear();
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();
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
//    transformVertex.clear();
//    glutPostRedisplay();
//}
//
//void InitialMenu()
//{
//    int rasMenu = glutCreateMenu(RasterizeMenu);
//    glutAddMenuEntry("Rasterize Line", 1);
//    glutAddMenuEntry("Rasterize Polygon", 2);
//
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
//    glutAddSubMenu("Rasterize Option", rasMenu);
//    glutAddSubMenu("Ras_Line Algorithm", algMenu);
//    glutAddSubMenu("Map Size", msMenu);
//    glutAttachMenu(GLUT_RIGHT_BUTTON);
//}
//
//#pragma endregion
