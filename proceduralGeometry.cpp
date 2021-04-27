#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#include "proceduralGeometry.h"
#include "camera.h"
#include "GridPlanesAndTextures.h"

matrix generateObjects(matrix array[], matrix lenght, float Min, float Max, int index);
void initPrism(int height, int slices, float radius, matrix **v, int **index);
void drawGeometry(int length, matrix *v, int *index, int n);

static matrix color[3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

static matrix vOrigin = {0, 0, 0};
static matrix *vCylinder = NULL;
static int *iCylinder = NULL;
float cube[8][3];
static GLubyte cubes[6][4] ={
    {3,2,1,0},
    {5, 6, 7, 4},
    {2,6,5,1},
    {0, 1, 5, 4},
    {3, 0, 4,7},
    {7, 6, 2, 3}
};
void drawGeometry(int length, matrix *v, int *index, int n)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < length; i++)
	{
		if (i % n == 0)
			glNormal3fv((float *) &v[index[i]]);
		glVertex3fv((float *) &v[index[i]]);
	}
	glEnd();
}

void initCylinder(void)
{
	initPrism(CHEIGHT, CSLICES, CRADIUS,
			&vCylinder, &iCylinder);
}

void drawCylinder(void)
{
	glColor3f(1, 0, 0);
	float length = CSLICES*6+(CSLICES-2)*6;
	drawGeometry(length/2, vCylinder, iCylinder, 3);
	drawAxes(1);
}
void initPrism(int height, int slices, float radius, matrix **v, int **index)
{
	*v = (matrix *) calloc(slices*2, sizeof(matrix));
	int nIndex = slices*6+(slices-2)*2*3;
	*index = (int *) calloc(nIndex, sizeof(int));
	if (!(*v) || !(*index))
	{
		exit(1);
	}

	float theta;
	int iCount = 0;

	/* Get the vertex */
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < slices; i++)
		{
			theta = i / (float)slices * 2.0 * M_PI;
			(*v)[j*slices + i].z = radius * cosf(theta+M_PI/4);
			(*v)[j*slices + i].y = radius * sinf(theta+M_PI/4);
			(*v)[j*slices + i].x = height * j - height/2;
		}
	}

	/* Get the vertex index for body */
	for (int i = 0; i < slices; i++)
	{
		(*index)[iCount++] = i;
		(*index)[iCount++] = slices+i;
		(*index)[iCount++] = slices+(i+1)%slices;

		(*index)[iCount++] = i;
		(*index)[iCount++] = slices+(i+1)%slices;
		(*index)[iCount++] = (i+1)%slices;
	}

	/* Get the vertex index for top and bottom */
	for (int j = 0; j < 2; j++)
	{
		for (int i = 1; i < slices - 1; i++)
		{
			(*index)[iCount++] = j*slices + 0;
			(*index)[iCount++] = j*slices + i;
			(*index)[iCount++] = j*slices + i+1;
		}
	}
}

void func(double x, double y, double z){

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<3; j++)
        {
            cube[i][j]=0.00;
        }
    }
    cube[1][2]=z,cube[2][0]=x,cube[2][2]=z,cube[3][0]=x,cube[4][1]=y,cube[5][1]=y,cube[5][2]=z;
    cube[6][0]=x,cube[6][1]=y,cube[6][2]=z,cube[7][0]=x,cube[7][1]=y;
}
void drawcube(double x,double y, double z){

    func(x,y,z);
    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++) {
        glVertex3fv(&cube[cubes[i][0]][0]);
        glVertex3fv(&cube[cubes[i][1]][0]);
        glVertex3fv(&cube[cubes[i][2]][0]);
        glVertex3fv(&cube[cubes[i][3]][0]);
    }
    glEnd();

}

void initCars(void)
{
	matrix length = {CUBE_LENGTH, CUBE_LENGTH, CUBE_LENGTH};
	for (int i = 0; i < 8; i++){
		cars[i] = generateObjects(cars, length, 0, 0, i);
        cars[i].z = 4;
	}

    for (int i = 8; i < 16; i++){
		cars[i] = generateObjects(cars, length, 0, 0, i);
        cars[i].z = 2.0;
	}

    for (int i = 16; i < 24; i++){
		cars[i] = generateObjects(cars, length, 0, 0, i);
        //cars[i].z = 2.0;
	}
    for (int i = 24; i < 32; i++){
		cars[i] = generateObjects(cars, length, 0, 0, i);
        cars[i].z = -2.0;
	}
}

void initLogs(void)
{
	matrix length = {CHEIGHT, CRADIUS*4, CRADIUS*2};
    for (int i = 0; i < 10; i++)
	{
        logs[i] = generateObjects(logs, length, 1, 2, i);
            logs[i].z = -8.4;
	}
    for (int i = 10; i < 20; i++)
	{
        logs[i] = generateObjects(logs, length, 1, 2, i);
            logs[i].z = -9.6;
	}
    for (int i = 20; i < 30; i++)
	{
        logs[i] = generateObjects(logs, length, 1, 2, i);
            logs[i].z = -11.3;
	}
    for (int i = 30; i < 40; i++)
	{
        logs[i] = generateObjects(logs, length, 1, 2, i);
            logs[i].z = -12;
	}
}
void drawCars(void)
{
	for (int i = 0; i < 32; i++)
	{
        glPushMatrix();
	    glTranslatef(cars[i].x, cars[i].y, cars[i].z);
	    //printf("%.2f\n", cars[i].z);
	    drawAxes(1);
        drawMainCarCubes();
        glPopMatrix();

	}
}

void drawMainCarCubes(void)
{
        glPushMatrix();
        glColor3f(0,1,2);
        glTranslated(0,0.2,0);
        drawcube(.4,0.3,.5);
        glPopMatrix();
        //
        glPushMatrix();
        glColor3f(1,1,.2);
        glTranslated(-0.2,0,0);
        drawcube(.9,0.2,.5);
        glPopMatrix();
}
void drawLogs(void)
{
    for (int i = 0; i < 40; i++)
	{

        glTranslatef(logs[i].x, getWaveY(i), logs[i].z); //- 13
		drawCylinder();
        glTranslatef(-logs[i].x, -getWaveY(i), -logs[i].z);
	}
    //printf("Log: %f,%f\n", logs[0].x, logs[0].z);
}


void updateCars(void)
{
	for (int i = 0; i < 32; i++)
		cars[i].x = (cars[i].x+CARSPEED < GENERATELENGTH/2 - CUBE_LENGTH) ?
			cars[i].x+CARSPEED : -GENERATELENGTH/2 + CUBE_LENGTH/2;
}

void updateLogs(void)
{
    for (int i = 0; i < 40; i++)
		logs[i].x = (logs[i].x+LOGSPEED < GENERATELENGTH/2 - CHEIGHT) ?
			logs[i].x+LOGSPEED : -GENERATELENGTH/2 + CHEIGHT/2;
}

matrix generateObjects(matrix array[], matrix length, float Min, float Max, int index)
{
	int i;
	matrix axes = {0, 0, 0};

	while (1)
	{
		axes.x = OBJGENERATION(-GENERATELENGTH/2 + length.x/2, GENERATELENGTH/2 - length.x/2);
		axes.z = OBJGENERATION(Min + length.z/2, Max - length.z/2);

		for (i = 0; i < index; i++)
			if (array[i].x >= axes.x-length.x/2 && array[i].x <= axes.x+length.x/2 &&
				array[i].z >= axes.z-length.z/2 && array[i].z <= axes.z+length.z/2){
				break;
				}
		if (i == index)
			break;
	}

	return axes;
}
void drawFrogLeg(void){
    glPushMatrix();
    drawcube(0.08,0.04,0.04);
    glPopMatrix();
}

void drawSphere(void)
{
    //printf("Frog: %f,%f\n", frog.r.x, frog.r.z);
    float headOffset = 0.2;
    glColor3f(0, 1, 0);
    glTranslatef(frog.r.x, frog.r.y, frog.r.z);

    glRotatef(frog.r0.phi/M_PI * 180, 0, 1, 0);
    glRotatef((M_PI/2) * 180, 0, 1, 0);
//Body
    glPushMatrix();
    drawcube(0.18,0.1,0.1);
    glPopMatrix();

//Head
    glTranslated(0.18,0.02,0.02);
    glPushMatrix();
    drawcube(0.03,0.06,0.06);
    glPopMatrix();
    glTranslated(-0.18,-0.03,-0.03);

//Front left leg
    glTranslated(0.11,0.0,-0.028);
    glRotatef(35,0,1,0);
    drawFrogLeg();
    glRotatef(-35,0,1,0);
    glTranslated(-0.11,0.0,0.028);

//Front right leg
    glTranslated(0.14,0.0,0.11);
    glRotatef(-35,0,1,0);
    drawFrogLeg();
    glRotatef(35,0,1,0);
    glTranslated(-0.14,0.0,-0.11);

//Back right leg
    glTranslated(0.0,0.0,0.11);
    glRotatef(-35,0,1,0);
    drawFrogLeg();
    glRotatef(35,0,1,0);
    glTranslated(0.0,0.0,-0.11);

//Back left leg
    glTranslated(0.0,0.0,-0.028);
    glRotatef(35,0,1,0);
    drawFrogLeg();
    glRotatef(-35,0,1,0);
    glTranslated(0.0,0.0,0.028);
    glRotatef(-(M_PI/2) * 180, 0, 1, 0);
    glRotatef(-frog.r0.phi/M_PI * 180, 0, 1, 0);
    drawAxes(1);
    glTranslatef(-frog.r.x, -frog.r.y, -frog.r.z);
}
