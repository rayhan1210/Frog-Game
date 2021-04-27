#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <typeinfo>
//#include <SOIL.h>

#define GL_SILENCE_DEPRECATION
#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#define DEBUG

#include "camera.h"
#include "input.h"
#include "jumpingFunctionality.h"
#include "GridPlanesAndTextures.h"
#include "proceduralGeometry.h"

void init(void);
void initScene(void);
void display(void);
void idle(void);
void reshape(int width, int height);
void initLighting(void);
void infoDisplay(void);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024, 1024);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	initScene();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	updateCartesian(&frog.r);
	updateCartesian(&frog.r0);

}

void initScene(void){

    initTextures();
	initCylinder();
    initCars();
	initLogs();
    initLighting();
}

void initLighting(void)
{
	float specular[] = {1, 1, 1, 1};
    float shininess[] = {100};
	float lPosition[] = {1, 1, 1, 0};
    float ambient[] = {1, 1, 0, 1};
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

void displayText(void){
	char buffer[30];
  char *bufp;
  int w, h;

  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  /* Set up orthographic coordinate system to match the window,
     i.e. (0,0)-(w,h) */
  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  /* Score */
  glColor3f(1.0, 1.0, 0.0);
  glRasterPos2i(w-150, h-20);
  snprintf(buffer, sizeof buffer, "Lives:  %i", getLives());
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

  /* Lives */
  glColor3f(1.0, 1.0, 0.0);
  glRasterPos2i(w-150, h-40);
  snprintf(buffer, sizeof buffer, "Score: %i", getScore());
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

  glPopMatrix();  /* Pop modelview */
  glMatrixMode(GL_PROJECTION);

  glPopMatrix();  /* Pop projection */
  glMatrixMode(GL_MODELVIEW);

  glPopAttrib();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setProjectionMatrix();
	setupCamera();
	GLfloat lightpos[] = {1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	if (getWireFlag())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    RiverBed();
    RiverWalls();
    waterGrid();
    grassAfteRiver();
    grassBforRoad();
    Road();
    grassAfterRoad();
    drawDirectionSpeedVector();
	drawParabola();
	drawParabolaNormalTangent();
	drawSphere();
    drawCars();
    drawLogs();
	displayText();
	glutSwapBuffers();
}

void idle(void)
{
	jumpingIdle();
	updateCars();
	updateLogs();
	glutPostRedisplay();
}


void reshape(int width, int height)
{
	glViewport(frog.r.x, frog.r.y, width, height);
	setProjectionMatrix();
	glutPostRedisplay();
	setWidth(width);
	setHeight(height);
}
