// Block.cpp
// OpenGL SuperBible, Chapter 1
// Demonstrates an assortment of basic 3D concepts
// Program by Richard S. Wright Jr.

#include <math.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include ".\GL\freeglut.h"

using namespace cv;

// Keep track of effects step
int nStep = 0;


// Lighting data
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat lightSpecular[] = { 0.9f, 0.9f, 0.9f };
GLfloat materialColor[] = { 0.8f, 0.0f, 0.0f };
GLfloat vLightPos[] = { -80.0f, 120.0f, 100.0f, 0.0f };
GLfloat ground[3][3] = { { 0.0f, -25.0f, 0.0f },
						{ 10.0f, -25.0f, 0.0f },
						{ 10.0f, -25.0f, -10.0f } };

GLuint textures[4];

GLfloat torsoWidth, torsoHeight;
GLfloat headRadius, jointRadius;
GLfloat armHeight, armWidth;
GLfloat legHeight, legWidth;

GLfloat luaRotateX, lfaRotateX;
GLfloat ruaRotateX, rfaRotateX;
GLfloat lulRotateX, ldlRotateX;
GLfloat rulRotateX, rdlRotateX;
GLfloat luaRotateY, lfaRotateY;
GLfloat ruaRotateY, rfaRotateY;
GLfloat lulRotateY, ldlRotateY;
GLfloat rulRotateY, rdlRotateY;
GLfloat luaRotateZ, lfaRotateZ;
GLfloat ruaRotateZ, rfaRotateZ;
GLfloat lulRotateZ, ldlRotateZ;
GLfloat rulRotateZ, rdlRotateZ;

void initPosition()
{
	headRadius = 15;
	jointRadius = 5;

	torsoHeight = 50.0f;
	torsoWidth = 30.0f;

	armHeight = 20;
	armWidth = 8;

	legHeight = 25;
	legWidth = 10;

	luaRotateY = lfaRotateY = ruaRotateY = rfaRotateY = lulRotateY = ldlRotateY = rulRotateY = rdlRotateY = 0;
	luaRotateZ = lfaRotateZ = ruaRotateZ = rfaRotateZ = lulRotateZ = ldlRotateZ = rulRotateZ = rdlRotateZ = 0;
	luaRotateX = lfaRotateX = ruaRotateX = rfaRotateX = lulRotateX = ldlRotateX = rulRotateX = rdlRotateX = 0;
}

void drawTorso()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	//back
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, torsoHeight, -15.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, -15.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(torsoWidth, 0, -15.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(torsoWidth, torsoHeight, -15.0f);
	glEnd();

	//bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, -15.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 15.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(torsoWidth, 0, 15.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(torsoWidth, 0, -15.0f);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, -15.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 15.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, torsoHeight, 15.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, torsoHeight, -15.0f);
	glEnd();

	//top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, torsoHeight, -15.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, torsoHeight, 15.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(torsoWidth, torsoHeight, 15.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(torsoWidth, torsoHeight, -15.0f);
	glEnd();

	//right
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(torsoWidth, torsoHeight, -15.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(torsoWidth, torsoHeight, 15.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(torsoWidth, 0, 15.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(torsoWidth, 0, -15.0f);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, torsoHeight, 15.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 15.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(torsoWidth, 0, 15.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(torsoWidth, torsoHeight, 15.0f);
	glEnd();

}

void drawHead()
{
	glColor3f(1, 0, 0);
	glutSolidSphere(10, 20, 20);
}

void drawJoint()
{
	glColor3f(0, 1, 0);
	glutSolidSphere(5, 20, 20);
}

void drawArm()
{
	glColor3ub(0, 170, 255);

	//back
	glBegin(GL_QUADS);
	glVertex3f(0, armHeight, -4.0f);
	glVertex3f(0, 0, -4.0f);
	glVertex3f(armWidth, 0, -4.0f);
	glVertex3f(armWidth, armHeight, -4.0f);
	glEnd();

	//bottom
	glBegin(GL_QUADS);
	glVertex3f(0, 0, -4.0f);
	glVertex3f(0, 0, 4.0f);
	glVertex3f(armWidth, 0, 4.0f);
	glVertex3f(armWidth, 0, -4.0f);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glVertex3f(0, 0, -4.0f);
	glVertex3f(0, 0, 4.0f);
	glVertex3f(0, armHeight, 4.0f);
	glVertex3f(0, armHeight, -4.0f);
	glEnd();

	//top
	glBegin(GL_QUADS);
	glVertex3f(0, armHeight, -4.0f);
	glVertex3f(0, armHeight, 4.0f);
	glVertex3f(armWidth, armHeight, 4.0f);
	glVertex3f(armWidth, armHeight, -4.0f);
	glEnd();

	//right
	glBegin(GL_QUADS);
	glVertex3f(armWidth, armHeight, -4.0f);
	glVertex3f(armWidth, armHeight, 4.0f);
	glVertex3f(armWidth, 0, 4.0f);
	glVertex3f(armWidth, 0, -4.0f);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glVertex3f(0, armHeight, 4.0f);
	glVertex3f(0, 0, 4.0f);
	glVertex3f(armWidth, 0, 4.0f);
	glVertex3f(armWidth, armHeight, 4.0f);
	glEnd();
}

void drawLeg()
{
	glColor3ub(140, 126, 115);

	//back
	glBegin(GL_QUADS);
	glVertex3f(0, legHeight, -5.0f);
	glVertex3f(0, 0, -5.0f);
	glVertex3f(legWidth, 0, -5.0f);
	glVertex3f(legWidth, legHeight, -5.0f);
	glEnd();

	//bottom
	glBegin(GL_QUADS);
	glVertex3f(0, 0, -5.0f);
	glVertex3f(0, 0, 5.0f);
	glVertex3f(legWidth, 0, 5.0f);
	glVertex3f(legWidth, 0, -5.0f);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glVertex3f(0, 0, -5.0f);
	glVertex3f(0, 0, 5.0f);
	glVertex3f(0, legHeight, 5.0f);
	glVertex3f(0, legHeight, -5.0f);
	glEnd();

	//top
	glBegin(GL_QUADS);
	glVertex3f(0, legHeight, -5.0f);
	glVertex3f(0, legHeight, 5.0f);
	glVertex3f(legWidth, legHeight, 5.0f);
	glVertex3f(legWidth, legHeight, -5.0f);
	glEnd();

	//right
	glBegin(GL_QUADS);
	glVertex3f(legWidth, legHeight, -5.0f);
	glVertex3f(legWidth, legHeight, 5.0f);
	glVertex3f(legWidth, 0, 5.0f);
	glVertex3f(legWidth, 0, -5.0f);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glVertex3f(0, legHeight, 5.0f);
	glVertex3f(0, 0, 5.0f);
	glVertex3f(legWidth, 0, 5.0f);
	glVertex3f(legWidth, legHeight, 5.0f);
	glEnd();
}

void DrawLab05()
{
	glPushMatrix();

	// Draw plane that the cube rests on
	glDisable(GL_LIGHTING);
	if (nStep == 5)
	{
		glColor3ub(255, 255, 255);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-100.0f, -25.3f, -100.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-100.0f, -25.3f, 100.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(100.0f, -25.3f, 100.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(100.0f, -25.3f, -100.0f);
		glEnd();
	}
	else
	{
		glColor3f(0.0f, 0.0f, 0.90f); // Blue
		glBegin(GL_QUADS);
		glVertex3f(-100.0f, -25.3f, -100.0f);
		glVertex3f(-100.0f, -25.3f, 100.0f);
		glVertex3f(100.0f, -25.3f, 100.0f);
		glVertex3f(100.0f, -25.3f, -100.0f);
		glEnd();
	}


	// Set drawing color to Red
	glColor3f(1.0f, 0.0f, 0.0f);

	// Enable, disable lighting
	if (nStep > 2)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_COLOR_MATERIAL);

		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glMaterialfv(GL_FRONT, GL_SPECULAR, lightSpecular);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
		glMateriali(GL_FRONT, GL_SHININESS, 128);
	}

	// Move the cube slightly forward and to the left
	glTranslatef(-10.0f, 0.0f, 10.0f);

	switch (nStep)
	{
		// Just draw the wire framed cube
	case 0:
		glutWireCube(50.0f);
		break;

		// Same wire cube with hidden line removal simulated
	case 1:
		// Front Face (before rotation)
		glBegin(GL_LINES);
		glVertex3f(25.0f, 25.0f, 25.0f);
		glVertex3f(25.0f, -25.0f, 25.0f);

		glVertex3f(25.0f, -25.0f, 25.0f);
		glVertex3f(-25.0f, -25.0f, 25.0f);

		glVertex3f(-25.0f, -25.0f, 25.0f);
		glVertex3f(-25.0f, 25.0f, 25.0f);

		glVertex3f(-25.0f, 25.0f, 25.0f);
		glVertex3f(25.0f, 25.0f, 25.0f);
		glEnd();

		// Top of cube
		glBegin(GL_LINES);
		// Front Face
		glVertex3f(25.0f, 25.0f, 25.0f);
		glVertex3f(25.0f, 25.0f, -25.0f);

		glVertex3f(25.0f, 25.0f, -25.0f);
		glVertex3f(-25.0f, 25.0f, -25.0f);

		glVertex3f(-25.0f, 25.0f, -25.0f);
		glVertex3f(-25.0f, 25.0f, 25.0f);

		glVertex3f(-25.0f, 25.0f, 25.0f);
		glVertex3f(25.0f, 25.0f, 25.0f);
		glEnd();

		// Last two segments for effect
		glBegin(GL_LINES);
		glVertex3f(25.0f, 25.0f, -25.0f);
		glVertex3f(25.0f, -25.0f, -25.0f);

		glVertex3f(25.0f, -25.0f, -25.0f);
		glVertex3f(25.0f, -25.0f, 25.0f);
		glEnd();

		break;

		// Uniform colored surface, looks 2D and goofey
	case 2:
		glutSolidCube(50.0f);
		break;

	case 3:
		glutSolidCube(50.0f);
		break;

		// Draw a shadow with some lighting
	case 4:
		glutSolidCube(50.0f);
		glPopMatrix();

		// Disable lighting, we'll just draw the shadow as black
		glDisable(GL_LIGHTING);

		glPushMatrix();

		glTranslatef(-10.0f, 0.0f, 10.0f);

		// Set drawing color to Black
		glColor3f(0.0f, 0.0f, 0.0f);

		glutSolidCube(50.0f);
		break;

	case 5:
		glColor3ub(255, 255, 255);

		// Front Face (before rotation)
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-25.0f, -25.0f, 25.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-25.0f, 25.0f, 25.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(25.0f, 25.0f, 25.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(25.0f, -25.0f, 25.0f);
		glEnd();

		// Top of cube
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(25.0f, 25.0f, 25.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(25.0f, 25.0f, -25.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-25.0f, 25.0f, -25.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-25.0f, 25.0f, 25.0f);
		glEnd();

		// Last two segments for effect
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(25.0f, 25.0f, 25.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(25.0f, 25.0f, -25.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(25.0f, -25.0f, -25.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(25.0f, -25.0f, 25.0f);
		glEnd();


		glPopMatrix();

		// Disable lighting, we'll just draw the shadow as black
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();

		glTranslatef(-10.0f, 0.0f, 10.0f);

		break;

	}

	glPopMatrix();
}

// Called to draw scene
void RenderScene(void)
{


	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	//Draw Start
	{
		glPushMatrix();
		glTranslatef(0, 0, 0);

		//Head
		{
			glPushMatrix();
			glTranslatef(torsoWidth / 2, torsoHeight + headRadius, 0);
			drawHead();
			glPopMatrix();
		}

		// Left Upper Arm Hand
		{
			glPushMatrix();

			glTranslatef(torsoWidth + jointRadius, torsoHeight - jointRadius, 0);
			glRotatef(luaRotateX, 1, 0, 0);
			glRotatef(luaRotateY, 0, 1, 0);
			glRotatef(luaRotateZ, 0, 0, 1);

			drawJoint();

			{
				glPushMatrix();
				glTranslatef(-armWidth / 2, -jointRadius - armHeight, 0);
				drawArm();

				// Left Front Arm
				{
					glPushMatrix();

					glTranslatef(+jointRadius, -jointRadius, 0);
					glRotatef(lfaRotateX, 1, 0, 0);
					glRotatef(lfaRotateY, 0, 1, 0);
					glRotatef(lfaRotateZ, 0, 0, 1);

					drawJoint();

					{
						glPushMatrix();
						glTranslatef(-armWidth / 2, -jointRadius - armHeight, 0);
						drawArm();
						glPopMatrix();
					}

					glPopMatrix();
				}

				glPopMatrix();
			}

			glPopMatrix();
		}

		// Right Upper Arm Hand
		{
			glPushMatrix();

			glTranslatef(-jointRadius, torsoHeight - jointRadius, 0);
			glRotatef(ruaRotateX, 1, 0, 0);
			glRotatef(ruaRotateY, 0, 1, 0);
			glRotatef(ruaRotateZ, 0, 0, 1);

			drawJoint();

			{
				glPushMatrix();
				glTranslatef(-armWidth / 2, -jointRadius - armHeight, 0);
				glRotatef(rfaRotateX, 1, 0, 0);
				glRotatef(rfaRotateY, 0, 1, 0);
				glRotatef(rfaRotateZ, 0, 0, 1);

				drawArm();

				// Right Front Arm
				{
					glPushMatrix();

					glTranslatef(+jointRadius, -jointRadius, 0);
					drawJoint();

					{
						glPushMatrix();
						glTranslatef(-armWidth / 2, -jointRadius - armHeight, 0);
						drawArm();
						glPopMatrix();
					}

					glPopMatrix();
				}

				glPopMatrix();
			}

			glPopMatrix();
		}

		// Left Upper Leg
		{
			glPushMatrix();
			glTranslatef((torsoWidth + torsoWidth / 2) / 2, -jointRadius, 0);
			glRotatef(lulRotateX, 1, 0, 0);
			glRotatef(lulRotateY, 0, 1, 0);
			glRotatef(lulRotateZ, 0, 0, 1);

			drawJoint();

			{
				glPushMatrix();
				glTranslatef(-legWidth / 2, -jointRadius - legHeight, 0);
				glRotatef(ldlRotateX, 1, 0, 0);
				glRotatef(ldlRotateY, 0, 1, 0);
				glRotatef(ldlRotateZ, 0, 0, 1);

				drawLeg();

				// down Leg
				{
					glPushMatrix();
					glTranslatef(legWidth / 2, -jointRadius, 0);
					drawJoint();

					{
						glPushMatrix();
						glTranslatef(-legWidth / 2, -jointRadius - legHeight, 0);
						drawLeg();

						glPopMatrix();
					}

					glPopMatrix();
				}

				glPopMatrix();
			}

			glPopMatrix();
		}

		// Right Upper Leg
		{
			glPushMatrix();
			glTranslatef((torsoWidth - torsoWidth / 2) / 2, -jointRadius, 0);
			glRotatef(rulRotateX, 1, 0, 0);
			glRotatef(rulRotateY, 0, 1, 0);
			glRotatef(rulRotateZ, 0, 0, 1);

			drawJoint();

			{
				glPushMatrix();
				glTranslatef(-legWidth / 2, -jointRadius - legHeight, 0);
				glRotatef(rdlRotateX, 1, 0, 0);
				glRotatef(rdlRotateY, 0, 1, 0);
				glRotatef(rdlRotateZ, 0, 0, 1);

				drawLeg();

				// down Leg
				{
					glPushMatrix();
					glTranslatef(legWidth / 2, -jointRadius, 0);
					drawJoint();

					{
						glPushMatrix();
						glTranslatef(-legWidth / 2, -jointRadius - legHeight, 0);
						drawLeg();

						glPopMatrix();
					}

					glPopMatrix();
				}

				glPopMatrix();
			}

			glPopMatrix();
		}

		drawTorso();
		glPopMatrix();
	}

	// Flush drawing commands
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	Mat image;

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenTextures(4, textures);

	// Load the texture objects
	image = imread("./material/lab07/floor.jpg");
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());

	image = imread("./material/lab07/Block6.jpg");
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());

	image = imread("./material/lab07/Block5.jpg");
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());

	image = imread("./material/lab07/Block4.jpg");
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());
}

void KeyPressFunc(unsigned char key, int x, int y)
{
	GLfloat shift = 5;
	switch (key)
	{
	case '1':
		luaRotateX += shift;
		break;
	case '2':
		luaRotateY += shift;
		break;
	case '3':
		luaRotateZ += shift;
		break;
	case 'q':
		lfaRotateX += shift;
		break;
	case 'w':
		lfaRotateY += shift;
		break;
	case 'e':
		lfaRotateZ += shift;
		break;
	case '5':
		ruaRotateX += shift;
		break;
	case '6':
		ruaRotateY += shift;
		break;
	case '7':
		ruaRotateZ += shift;
		break;
	case 't':
		rfaRotateX += shift;
		break;
	case 'y':
		rfaRotateY += shift;
		break;
	case 'u':
		rfaRotateZ += shift;
		break;
	case 'a':
		lulRotateX += shift;
		break;
	case 's':
		lulRotateY += shift;
		break;
	case 'd':
		lulRotateZ += shift;
		break;
	case 'z':
		ldlRotateX += shift;
		break;
	case 'x':
		ldlRotateY += shift;
		break;
	case 'c':
		ldlRotateZ += shift;
		break;
	case 'g':
		rulRotateX += shift;
		break;
	case 'h':
		rulRotateY += shift;
		break;
	case 'j':
		rulRotateZ += shift;
		break;
	case 'b':
		rdlRotateX += shift;
		break;
	case 'n':
		rdlRotateY += shift;
		break;
	case 'm':
		rdlRotateZ += shift;
		break;
	case 'r':
		luaRotateY = lfaRotateY = ruaRotateY = rfaRotateY = lulRotateY = ldlRotateY = rulRotateY = rdlRotateY = 0;
		luaRotateZ = lfaRotateZ = ruaRotateZ = rfaRotateZ = lulRotateZ = ldlRotateZ = rulRotateZ = rdlRotateZ = 0;
		luaRotateX = lfaRotateX = ruaRotateX = rfaRotateX = lulRotateX = ldlRotateX = rulRotateX = rdlRotateX = 0;
		break;
	default:
		break;
	}

	// Refresh the Window
	glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
	// Calculate new clipping volume
	GLfloat windowWidth;
	GLfloat windowHeight;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	// Keep the square square
	if (w <= h)
	{
		windowHeight = 100.0f * (GLfloat)h / (GLfloat)w;
		windowWidth = 100.0f;
	}
	else
	{
		windowWidth = 100.0f * (GLfloat)w / (GLfloat)h;
		windowHeight = 100.0f;
	}

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	glOrtho(-100.0f, windowWidth, -100.0f, windowHeight, -200.0f, 200.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION, vLightPos);

	//glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	//glRotatef(330.0f, 0.0f, 1.0f, 0.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab07_Texture");
	initPosition();
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(KeyPressFunc);
	glutDisplayFunc(RenderScene);

	SetupRC();

	glutMainLoop();
	glDeleteTextures(4, textures);
	return 0;
}
