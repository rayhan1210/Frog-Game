#include <stdio.h>

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

#include "camera.h"
/*
frogState frog = {
//	{   x,   y,   z,   r,  theta,    phi,  dx,  dy,  dz}
	{ 0.0, 0.0, 0.0, 2.0, M_PI/6, M_PI/4, 0.0, 0.0, 0.0},
	{ 0.0, 0.0, 0.0, 2.0, M_PI/6, M_PI/4, 0.0, 0.0, 0.0}
};*/
frogState frog = {
/*	{   x,   y,   z,   r,  theta,    phi,  dx,  dy,  dz}*/
	{ 0.0, 0.0, 7.0, 2.0, M_PI/6, M_PI/4, 0.0, 0.0, 0.0},
	{ 0.0, 0.0, 7.0, 2.0, M_PI/6, M_PI/4, 0.0, 0.0, 0.0} //making frog start from one end use 5, 5.
};

matrix cars[32];
matrix logs[40];

static int segments = 10;

static float width = 800;
static float height = 800;
static float rotateCamX = 45;
static float rotateCamY = 45;
static float camZoom = 1;
int score = 0;
int lives = 10;

static bool debug = true;
static bool cartesianFlag = true;
static bool tangentFlag = true;
static bool normalFlag = true;
static bool wireFlag = true;
static bool axesFlag = true;

void setProjectionMatrix(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, (width / height), 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void setupCamera(void)
{
	float camX, camY, camZ;
	float offsetYcamX, offsetYcamY, offsetYcamZ;
	float upX, upY, upZ;
	const float offset = 1.0;

	camX = camZoom * sin(rotateCamX*M_PI/180) * sin(rotateCamY*M_PI/180);
	camZ = camZoom * sin(rotateCamX*M_PI/180) * cos(rotateCamY*M_PI/180);
	camY = camZoom * cos(rotateCamX*M_PI/180);

	// Changing on the y to get a new up vector
	offsetYcamX = camZoom * sin(rotateCamX*M_PI/180-offset) *
		sin(rotateCamY*M_PI/180);
	offsetYcamZ = camZoom * sin(rotateCamX*M_PI/180-offset) *
		cos(rotateCamY*M_PI/180);
	offsetYcamY = camZoom * cos(rotateCamX*M_PI/180-offset);

	upX = offsetYcamX-camX;
	upY = offsetYcamY-camY;
	upZ = offsetYcamZ-camZ;


	glLoadIdentity();
	gluLookAt(frog.r.x + camX, frog.r.y + camY, frog.r.z + camZ,
			  frog.r.x, frog.r.y, frog.r.z,
			  upX, upY, upZ);
}

void setSegments(int newSegments)
{
	segments = newSegments;
}

void setWidth(float newWidth)
{
	width = newWidth;
}

void setHeight(float newHeight)
{
	height = newHeight;
}

void setRotateCamX(float newRotateCamX)
{
	rotateCamX = (newRotateCamX > 0 && newRotateCamX < 180)
		         ? newRotateCamX : rotateCamX;
}

void setRotateCamY(float newRotateCamY)
{
	rotateCamY = newRotateCamY;
}

void setCamZoom(float newCamZoom)
{
	camZoom = newCamZoom;
}

void setScore(int a){
	score = a;
}

void setLives(int a){
	lives = a;
}

int getScore(void){
	return score;
}

int getLives(void){
	return lives;
}

void updateCartesian(projection *p)
{
	p->dz = p->r * sin(p->theta) * cos(p->phi);
	p->dx = p->r * sin(p->theta) * sin(p->phi);
	p->dy = p->r * cos(p->theta);
}

void updateSpherical(projection *p)
{
	p->r = sqrt(p->dz*p->dz + p->dx*p->dx + p->dy*p->dy);
	p->theta = acos(p->dy / p->r);
	p->phi = atan2( p->dx, p->dz );
	//p->phi += ( p->phi < 0 ) ? 2*M_PI : 0;
}


void switchDebug(void)
{
	debug = !debug;
}

void switchCartesianFlag(void)
{
	cartesianFlag = !cartesianFlag;
}

void switchTangentFlag(void)
{
	tangentFlag = !tangentFlag;
}

void switchNormalFlag(void)
{
	normalFlag = !normalFlag;
}

void switchWireFlag(void)
{
	wireFlag = !wireFlag;
}

void switchAxesFlag(void)
{
	axesFlag = !axesFlag;
}

int getSegments(void)
{
	return segments;
}

float getWidth(void)
{
	return width;
}

float getHeight(void)
{
	return height;
}

float getRotateCamX(void)
{
	return rotateCamX;
}

float getRotateCamY(void)
{
	return rotateCamY;
}

float getCamZoom(void)
{
	return camZoom;
}

bool getDebug(void)
{
	return debug;
}

bool getCartesianFlag(void)
{
	return cartesianFlag;
}

bool getTangentFlag(void)
{
	return tangentFlag;
}

bool getNormalFlag(void)
{
	return normalFlag;
}

bool getWireFlag(void)
{
	return wireFlag;
}

bool getAxesFlag(void)
{
	return axesFlag;
}
