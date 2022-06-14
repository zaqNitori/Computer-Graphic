//
//#include <stdio.h> 
//#include <stdlib.h>
//#include <set>
//#include <cmath>
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
//typedef pair<int, int> pii;
//
//set<pii> coorSet;
//int menuSize;
//GLfloat range;
//GLfloat window_width, window_height;
//GLfloat orthoL, orthoR, orthoT, orthoB;
//
//void ChangeSize(int, int);
//void RenderScene();
//void mySpecialKey(int, int, int);
//void myKeyboard(unsigned char, int, int);
//void myMouse(int, int, int, int);
//void myMenu(int);
//
//void Reset()
//{
//    menuSize = 10;
//    orthoL = orthoB = -10;
//    orthoR = orthoT = 10;
//    range = window_width / menuSize / 2;
//}
//
//void InitialParameter()
//{
//    //初始化參數
//    window_height = window_width = 600;
//    Reset();
//}
//
//void InitialMenu()
//{
//    //右鍵 popup menu 設定
//    glutCreateMenu(myMenu);
//    glutAddMenuEntry("10", 10);
//    glutAddMenuEntry("15", 15);
//    glutAddMenuEntry("20", 20);
//    glutAddMenuEntry("25", 25);
//    glutAttachMenu(GLUT_RIGHT_BUTTON);
//}
//
//void InitialGlut(int argc, char** argv)
//{
//    //初始化GLUT
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowSize(window_width, window_height);
//    glutInitWindowPosition(600, 80);
//    glutCreateWindow("2D_Grid");
//}
//
//int main(int argc, char** argv)
//{
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
//void DrawMesh()
//{
//    //顯示原點(0, 0)
//    glColor3f(0, 0, 0);
//    glPointSize(4.0f);
//    glBegin(GL_POINTS);
//    glVertex3f(0, 0, 0);
//    glEnd();
//
//    //繪製表格
//    for (float i = -menuSize; i <= menuSize ; i++)
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
//void FillMesh()
//{
//    //填滿點選表格
//    int tx, ty;
//    for (auto au : coorSet)
//    {
//        //根據四個象限不同， 繪製的順序也不同
//        tx = (au.fi >= 0 ? au.fi - 1 : au.fi + 1);
//        ty = (au.se >= 0 ? au.se - 1 : au.se + 1);
//
//        glBegin(GL_QUADS);
//        glColor3f(1, 0, 0);
//        glVertex3f(au.fi, au.se, 0);
//        glVertex3f(au.fi, ty, 0);
//        glVertex3f(tx, ty, 0);
//        glVertex3f(tx, au.se, 0);
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
//    FillMesh();
//    DrawMesh();
//
//    glutSwapBuffers();
//}
//
//void myKeyboard(unsigned char key, int x, int y)
//{
//    glutPostRedisplay();
//}
//
//void SetOrtho(int size)
//{
//    // 修改捕捉範圍
//    orthoB = orthoL = -size;
//    orthoR = orthoT = size;
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(orthoL, orthoR, orthoB, orthoT, -10, 10);
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
//            printf("%.2f, %.2f\n", nx, ny);
//
//            //將轉換過後的座標轉成點選到的格狀座標
//            mx = (nx >= 0 ? (nx + range - 1) : (nx - range + 1)) / range;
//            my = (ny >= 0 ? (ny + range - 1) : (ny - range + 1)) / range;
//            printf("%d, %d\n\n", mx, my);
//
//            //判斷是否點選過， 以此來決定該格要不要塗色
//            pii tp = mkpr(mx, my);
//            if (coorSet.find(tp) == coorSet.end())
//                coorSet.insert(mkpr(mx, my));
//            else
//                coorSet.erase(tp);
//        }
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();
//}
//
//void myMenu(int index)
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
//
//    //格子數量有修改， 就要重新設定參數
//    SetOrtho(menuSize);
//    range = window_width / menuSize / 2;
//    coorSet.clear();
//
//    glutPostRedisplay();
//}
//
//void mySpecialKey(int key, int x, int y)
//{
//    glutPostRedisplay();
//}
//
//
