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
//typedef pair<int, int> pii;
//typedef pair<float, float> pff;
//
//enum PolygonMode {
//    Line = 1,
//    Fill = 2
//};
//
//#define X .525731112119133606
//#define Z .850650808352039932
//static GLfloat vdata[12][3] = {
//{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
//{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
//{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
//};
//
//static GLuint tindices[20][3] = {
//{1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
//{1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
//{3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
//{10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
//};
//
//
//PolygonMode pMode;
//float thetaX, thetaY, thetaZ;
//int polygonDepth;
//GLfloat window_width, window_height;
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
//    window_height = 600;
//    window_width = 1200;
//    pMode = PolygonMode::Line;
//    polygonDepth = 1;
//}
//
//void InitialGlut(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowSize(window_width, window_height);
//    glutInitWindowPosition(100, 80);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glutCreateWindow("Lab08_SubDivision");
//}
//
//void LightSetting()
//{
//    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
//    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//}
//
//void MaterialSetting()
//{
//    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
//    GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
//    GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
//    GLfloat mat_shininess = { 20.0 };
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
//    //glShadeModel(GL_SMOOTH); /*enable smooth shading */
//    glEnable(GL_LIGHTING); /* enable lighting */
//    glEnable(GL_LIGHT0); /* enable light 0 */
//}
//
//int main(int argc, char** argv)
//{
//    srand(time(NULL));
//    InitialParameter();
//    InitialGlut(argc, argv);
//    InitialMenu();
//    LightSetting();
//    MaterialSetting();
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
//    int ortho = 3;
//    int width = 2.5 * ortho;
//    printf("Window Size= %d X %d\n", w, h);
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-width, 0.5*ortho, -ortho, ortho, -ortho, ortho);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}
//
//#pragma endregion
//
//#pragma region Draw Method
//
//void normalize(float v[3])
//{
//    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
//    v[0] /= d; v[1] /= d; v[2] /= d;
//}
//
//void normCrossProd(float u[3], float v[3], float out[3])
//{
//    out[0] = u[1] * v[2] - u[2] * v[1];
//    out[1] = u[2] * v[0] - u[0] * v[2];
//    out[2] = u[0] * v[1] - u[1] * v[0];
//    normalize(out);
//}
//
//void icoNormVec(int i)
//{
//    GLfloat d1[3], d2[3], n[3];
//    for (int k = 0; k < 3; k++) 
//    {
//        d1[k] = vdata[tindices[i][0]][k] - vdata[tindices[i][1]][k];
//        d2[k] = vdata[tindices[i][1]][k] - vdata[tindices[i][2]][k];
//    }
//    normCrossProd(d1, d2, n);
//    glNormal3fv(n);
//}
//
//void DrawIcosahedron()
//{
//    glShadeModel(GL_FLAT);
//
//    glPushMatrix();
//    glTranslatef(-6, 0, 0);
//    glRotatef(thetaX, 1, 0, 0);
//    glRotatef(thetaY, 0, 1, 0);
//
//    glBegin(GL_TRIANGLES);
//    for (int i = 0; i < 20; i++)
//    {
//        icoNormVec(i);
//        glVertex3fv(&vdata[tindices[i][0]][0]);
//        glVertex3fv(&vdata[tindices[i][1]][0]);
//        glVertex3fv(&vdata[tindices[i][2]][0]);
//    }
//    glEnd();
//    glPopMatrix();
//}
//
//void DrawSphere()
//{
//    glShadeModel(GL_SMOOTH);
//
//    glPushMatrix();
//    glTranslatef(-3, 0, 0);
//    glRotatef(thetaX, 1, 0, 0);
//    glRotatef(thetaY, 0, 1, 0);
//
//    glBegin(GL_TRIANGLES);
//    for (int i = 0; i < 20; i++)
//    {
//        glNormal3fv(&vdata[tindices[i][0]][0]);
//        glVertex3fv(&vdata[tindices[i][0]][0]);
//        glNormal3fv(&vdata[tindices[i][1]][0]);
//        glVertex3fv(&vdata[tindices[i][1]][0]);
//        glNormal3fv(&vdata[tindices[i][2]][0]);
//        glVertex3fv(&vdata[tindices[i][2]][0]);
//    }
//    glEnd();
//    glPopMatrix();
//}
//
//void drawTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3])
//{
//    glBegin(GL_TRIANGLES);
//    glVertex3fv(v1);
//    glVertex3fv(v2);
//    glVertex3fv(v3);
//    glEnd();
//}
//
//void subdivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int depth)
//{
//    GLfloat v12[3], v23[3], v31[3]; int i;
//
//    if (depth == 0) {
//        drawTriangle(v1, v2, v3); //Draw if no further subdivision requested
//        return;
//    }
//
//    for (i = 0; i < 3; i++) {
//        v12[i] = (v1[i] + v2[i]) / 2.0;
//        v23[i] = (v2[i] + v3[i]) / 2.0;
//        v31[i] = (v3[i] + v1[i]) / 2.0;
//    }
//
//    normalize(v12);
//    normalize(v23);
//    normalize(v31);
//    subdivide(v1, v12, v31, depth - 1);
//    subdivide(v2, v23, v12, depth - 1);
//    subdivide(v3, v31, v23, depth - 1);
//    subdivide(v12, v23, v31, depth - 1);
//}
//
//void DrawSubvision(void)
//{
//    glPushMatrix();
//    glRotatef(thetaX, 1, 0, 0);
//    glRotatef(thetaY, 0, 1, 0);
//
//    for (int i = 0; i < 20; i++)
//    {
//        subdivide(&vdata[tindices[i][0]][0],
//            &vdata[tindices[i][1]][0],
//            &vdata[tindices[i][2]][0],
//            polygonDepth);
//    }
//    glPopMatrix();
//    glFlush();
//
//}
//
//void RenderScene()
//{
//    glClearColor(0, 0, 0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0, 0, 3.0f
//        , 0, 0, 0
//        , 0, 1, 0);
//
//    if(pMode == PolygonMode::Line)
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    else
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//    DrawSubvision();
//    DrawSphere();
//    DrawIcosahedron();
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
//        thetaX = thetaY = 0;
//        break;
//    case '+':
//        if (polygonDepth < 5)
//            polygonDepth++;
//        break;
//    case '-':
//        if (polygonDepth > 0)
//            polygonDepth--;
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();
//}
//
//void myMouse(int button, int state, int x, int y)
//{
//
//    switch (button)
//    {
//
//    default:
//        break;
//    }
//
//    glutPostRedisplay();
//}
//
//void mySpecialKey(int key, int x, int y)
//{
//    float shift = 4.0f;
//    switch (key)
//    {
//    case GLUT_KEY_LEFT:
//        thetaY -= shift;
//        break;
//    case GLUT_KEY_RIGHT:
//        thetaY += shift;
//        break;
//    case GLUT_KEY_UP:
//        thetaX -= shift;
//        break;
//    case GLUT_KEY_DOWN:
//        thetaX += shift;
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();
//}
//
//#pragma endregion
//
//
//#pragma region Menu
//
//void myMenu(int index)
//{
//    //
//}
//
//void modeMenu(int index)
//{
//    switch (index)
//    {
//    case PolygonMode::Line:
//        pMode = PolygonMode::Line;
//        break;
//    case PolygonMode::Fill:
//        pMode = PolygonMode::Fill;
//        break;
//    default:
//        break;
//    }
//
//    glutPostRedisplay();
//}
//
//void InitialMenu()
//{
//    int menu = glutCreateMenu(modeMenu);
//    glutAddMenuEntry("Line", PolygonMode::Line);
//    glutAddMenuEntry("Fill", PolygonMode::Fill);
//
//    glutCreateMenu(myMenu);
//    glutAddSubMenu("Polygon Mode", menu);
//    glutAttachMenu(GLUT_RIGHT_BUTTON);
//}
//
//#pragma endregion
