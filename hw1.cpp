
//Computer Graphics HW1
//Transformation and Projection

#include <math.h>
#include <stdlib.h>

#include "GLee.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include "swgl.h"
#include "font.h"

#ifndef bool
#define bool int
#define false 0
#define true 1
#endif

#ifndef M_PI
#define M_PI 3.14159
#endif


int 	winWidth, winHeight;

float 	angle = 0.0, axis[3], trans[3];
bool 	trackingMouse = false;
bool 	redrawContinue = false;
bool    trackballMove = false;
GLdouble TRACKM[16]={1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

GLdouble Angle1=0, Angle2=0;
GLint TICK=0;

GLFONT *Font;
/*----------------------------------------------------------------------*/
/*
** Draw the wireflame cube.
*/
GLfloat vertices[][3] = {
    {-1.0,-1.0,-1.0},{1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
    {-1.0,-1.0,1.0}, {1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}
};

GLfloat colors[][3] = {
    {0.0,0.0,0.0},{1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
    {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}
};


void SwglLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2)
{
	//copy to homogenous coordinate
	GLdouble h1[4]={x1, y1, z1, 1.0};
	GLdouble h2[4]={x2, y2, z2, 1.0};

	GLdouble w1[4]={x1, y1, 0, 1.0}; //window coordinate
	GLdouble w2[4]={x2, y2, 0, 1.0};

	//implement the opengl pipeline here
	swTransformation(h1, w1);
	swTransformation(h2, w2);

	//draw the 2D line
	glBegin(GL_LINES);
		//glColor3fv(colors[index1]);
		glVertex2f(w1[0], w1[1]);
		//glColor3fv(colors[index2]);
		glVertex2f(w2[0], w2[1]);
	glEnd();
}

void SwglLine(int index1, int index2)
{
	SwglLine(vertices[index1][0], vertices[index1][1], vertices[index1][2],
		     vertices[index2][0], vertices[index2][1], vertices[index2][2]);
}

void OpenglLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2)
{
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
}

void OpenglLine(int index1, int index2)
{
	OpenglLine(vertices[index1][0], vertices[index1][1], vertices[index1][2],
		       vertices[index2][0], vertices[index2][1], vertices[index2][2]);
}

void WireQuad(int a, int b, int c , int d, bool USINGOPENGL)
{
	if(USINGOPENGL) {
		OpenglLine(a, b);
		OpenglLine(b, c);
		OpenglLine(c, d);
		OpenglLine(d, a);
	} else {
		SwglLine(a, b);
		SwglLine(b, c);
		SwglLine(c, d);
		SwglLine(d, a);
	}
}

void swWireCube(void)
{
    // map vertices to faces */
    WireQuad(1,0,3,2, false);
    WireQuad(3,7,6,2, false);
    WireQuad(7,3,0,4, false);
    WireQuad(2,6,5,1, false);
    WireQuad(4,5,6,7, false);
    WireQuad(5,4,0,1, false);
}

void glWireCube(void)
{
    // map vertices to faces */
    WireQuad(1,0,3,2, true);
    WireQuad(3,7,6,2, true);
    WireQuad(7,3,0,4, true);
    WireQuad(2,6,5,1, true);
    WireQuad(4,5,6,7, true);
    WireQuad(5,4,0,1, true);
}

void polygon(int a, int b, int c , int d, int face)
{
    /* draw a polygon via list of vertices */
    glBegin(GL_POLYGON);
  	glColor3fv(colors[a]);
  	glVertex3fv(vertices[a]);
  	glColor3fv(colors[b]);
  	glVertex3fv(vertices[b]);
  	glColor3fv(colors[c]);
  	glVertex3fv(vertices[c]);
  	glColor3fv(colors[d]);
  	glVertex3fv(vertices[d]);
    glEnd();
}

void colorcube(void)
{

    /* map vertices to faces */
    polygon(1,0,3,2,0);
    polygon(3,7,6,2,1);
    polygon(7,3,0,4,2);
    polygon(2,6,5,1,3);
    polygon(4,5,6,7,4);
    polygon(5,4,0,1,5);
}



void softPath(void)
{
    //Do not change, setting a basic transformation
	glViewport(0, 0, winWidth, winHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glOrtho(0, winWidth, 0, winHeight, -2.0, 2.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 0, 0);
	OpenglLine(winWidth/2, 0, 0, winWidth, winHeight, 0);


	//
	//replace the opengl function in openglPath() to sotfgl
    //
/*
	//view transform
	swViewport(winWidth/2, 0, winWidth/2, winHeight);

    swMatrixMode(GL_PROJECTION);
    swLoadIdentity();
	//swOrtho(-2.0, 2.0, -2.0, 2.0, -3.0, 3.0);
	swuPerspective(60, (GLfloat)(winWidth*0.5)/winHeight, 0.1, 25);

    swMatrixMode(GL_MODELVIEW);
	swLoadIdentity();
	swuLookAt(5, 5, 20, 0, 5, 0, 0, 1, 0);

	//world coordinate
	glColor3f(1, 0, 0);
	SwglLine(0, 0, 0, 1, 0, 0);
	glColor3f(0, 1, 0);
	SwglLine(0, 0, 0, 0, 1, 0);
	glColor3f(0, 0, 1);
	SwglLine(0, 0, 0, 0, 0, 1);

	//multiple trackball matrix
	swMultMatrixd(TRACKM);

	swPushMatrix();
		swScaled(1, 2, 1);
		glColor3f(1, 0, 0);
		swWireCube();
	swPopMatrix();


	swTranslated(0, 2, 0);
	swRotated(Angle1, 0, 0, 1);
	swRotated(Angle2, 0, 1, 0);
		swPushMatrix();
			swTranslated(0, 2, 0);
			swScaled(1, 2, 1);
			glColor3f(0, 1, 0);
			swWireCube();
		swPopMatrix();

		swTranslated(0, 4, 0);
		swRotated(Angle1, 0, 0, 1);

		swPushMatrix();
			swTranslated(0, 2, 0);
			swScaled(1, 2, 1);
			glColor3f(0, 0, 1);
			swWireCube();
		swPopMatrix();
*/

}

void openglPath(void)
{
    //view transform
	glViewport(0, 0, winWidth/2, winHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -3.0, 3.0);
	gluPerspective(60, (GLfloat)(winWidth*0.5)/winHeight, 0.1, 25);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5, 5, 20, 0, 5, 0, 0, 1, 0);

	//world coordinate
	glColor3f(1, 0, 0);
	OpenglLine(0, 0, 0, 1, 0, 0);
	glColor3f(0, 1, 0);
	OpenglLine(0, 0, 0, 0, 1, 0);
	glColor3f(0, 0, 1);
	OpenglLine(0, 0, 0, 0, 0, 1);

	//multiple trackball matrix
	glMultMatrixd(TRACKM);

	glPushMatrix();
		glScaled(1, 2, 1);
		glColor3f(1, 0, 0);
		glWireCube();
	glPopMatrix();


	glTranslated(0, 2, 0);
	glRotated(Angle1, 0, 0, 1);
	glRotated(Angle2, 0, 1, 0);
		glPushMatrix();
			glTranslated(0, 2, 0);
			glScaled(1, 2, 1);
			glColor3f(0, 1, 0);
			glWireCube();
		glPopMatrix();

		glTranslated(0, 4, 0);
		glRotated(Angle1, 0, 0, 1);

		glPushMatrix();
			glTranslated(0, 2, 0);
			glScaled(1, 2, 1);
			glColor3f(0, 0, 1);
			glWireCube();
		glPopMatrix();

}

/*----------------------------------------------------------------------*/
/*
** These functions implement a simple trackball-like motion control.
*/

float lastPos[3] = {0.0F, 0.0F, 0.0F};
int curx, cury;
int startX, startY;

void trackball_ptov(int x, int y, int width, int height, float v[3])
{
    float d, a;

    /* project x,y onto a hemi-sphere centered within width, height */
    v[0] = (2.0F*x - width) / width;
    v[1] = (height - 2.0F*y) / height;
    d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
    v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
    a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}


void mouseMotion(int x, int y)
{
    float curPos[3], dx, dy, dz;

    trackball_ptov(x, y, winWidth, winHeight, curPos);
	if(trackingMouse)
	{
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];

		if (dx || dy || dz) {
			angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);

			axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
			axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
			axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
	}
    glutPostRedisplay();
}

void startMotion(int x, int y)
{
    trackingMouse = true;
    redrawContinue = false;
    startX = x; startY = y;
    curx = x; cury = y;
    trackball_ptov(x, y, winWidth, winHeight, lastPos);
	trackballMove=true;
}

void stopMotion(int x, int y)
{
	trackingMouse = false;

    if (startX != x || startY != y) {
		redrawContinue = true;
    } else {
		angle = 0.0F;
		redrawContinue = false;
		trackballMove = false;
    }
}

/*----------------------------------------------------------------------*/

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //Font
	glColor3f(1.0, 1.0, 1.0);  //set font color
	glWindowPos2i(10, winHeight-20);    //set font start position
    FontPrintf(Font, 1, "OpenGL");  //print font in screen
	glWindowPos2i(10+(winWidth/2), winHeight-20);    //set font start position
    FontPrintf(Font, 1, "SoftGL");  //print font in screen

    if (trackballMove) {
		glPushMatrix();
			glLoadMatrixd(TRACKM);
			glRotatef(angle, axis[0], axis[1], axis[2]);
			glGetDoublev(GL_MODELVIEW_MATRIX, TRACKM);
		glPopMatrix();
	}

	openglPath();

	softPath();

    glutSwapBuffers();
}

/*----------------------------------------------------------------------*/

void mouseButton(int button, int state, int x, int y)
{
	if(button==GLUT_RIGHT_BUTTON) exit(0);
	if(button==GLUT_LEFT_BUTTON) switch(state)
	{
		case GLUT_DOWN:
			y=winHeight-y;
			startMotion(x, y);
			break;
		case GLUT_UP:
			stopMotion(x, y);
			break;
    }
}

void myReshape(int w, int h)
{
    winWidth = w;
    winHeight = h;
}

void spinCube()
{
    if (redrawContinue) glutPostRedisplay();
}

void update(int i)
{
	TICK++;
	int temp=TICK%180;
	if(temp<90)
		Angle1++;
	else
		Angle1--;

	//int temp2=TICK%90;
	if(temp<90)
		Angle2+=0.5;
	else
		Angle2-=0.5;

	glutPostRedisplay();
	glutTimerFunc(33, update, ++i);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 400);
    glutCreateWindow("CG_HW1");

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(spinCube);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
	glutTimerFunc(33, update, 0);

	glEnable(GL_DEPTH_TEST);
	Font = FontCreate(wglGetCurrentDC(), "Times", 32, 0, 1);


    glutMainLoop();

	return 0;
}


