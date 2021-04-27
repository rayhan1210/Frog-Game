#include <stdio.h>
#include <stdlib.h> // pulls in declaration of malloc, free
#include <string.h> // pulls in declaration for strlen.
#include "SOIL.h"
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

#include "GridPlanesAndTextures.h"
#include "camera.h"



static matrix axes[3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

static matrix axesPoints = {0, 0, 0};
//static vertex *vCylinder = NULL;
//static int *iCylinder = NULL;
float totalGd = GRIDSIZE+GRIDSIZE;

static const GLfloat squareTexCoord[] = {
        0, 1,
        1, 1,
        0, 0,
        1, 0,
 };
static GLuint loadTexture(const char *filename){
	GLuint tex = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (!tex)
		return 0;

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}
void drawNormalLines(float x, float y, float z, float i, float j, float k, float a, float b, float c){
  glBegin(GL_LINES);
  glColor3f(a, b, c);
  glVertex3f(x, y, z);
  glVertex3f(x + i, y + j, z + k);
  glEnd();
}
void addTransparency(color3f c, float alpha){
    float material[] = {c.redA, c.greenA, c.blueA, alpha};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);

}
void initTextures(){
    texA.road = loadTexture("road.png");
    //printf("loaded texture %d\n", texA.road);
    texA.groundA = loadTexture("grass.jpg");
    texA.groundB = loadTexture("grass.jpg");
    texA.sandA = loadTexture("sand.jpg");}

void drawAxes(float scale)
{
	if (!getAxesFlag())
		return;

	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++)
	{
		glColor3fv((float*) &axes[i]);
		glVertex3fv((float*) &axesPoints);
		glVertex3f(scale*axes[i].x, scale*axes[i].y, scale*axes[i].z);
	}
	glEnd();

}

float getWaveY(float x){
    float river_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - 0;
    return rvr.A * sinf((x*rvr.k) + river_time) ;
}

void waterGrid(){
    float depth = 0.1;

    for (float i = -rvr.startXpos; i< rvr.endXpos; i++) {
        for (float j = -rvr.startZpos; j < -rvr.endZpos; j++) {
            glDisable(GL_LIGHTING);
            glBegin(GL_TRIANGLES);
            glColor4f(0.0, 0.5, 1.0, .5);
            glNormal3f(0, 1, 0);
            glVertex3f(i,getWaveY(i)-depth,j); //1
            glNormal3f(0, 1, 0);
            glVertex3f(i,getWaveY(i)-depth,j+1);//2
            glNormal3f(0, 1, 0);
            glVertex3f(i+1,getWaveY(i+1)-depth,j);//3
            glNormal3f(0, 1, 0);
            glVertex3f(i,getWaveY(i)-depth,j+1);//4
            glNormal3f(0, 1, 0);
            glVertex3f(i+1,getWaveY(i+1)-depth,j);//5
            glNormal3f(0, 1, 0);
            glVertex3f(i+1,getWaveY(i+1)-depth,j+1); //6
            glEnd();
            if(getNormalFlag()){
                drawNormalLines(i, getWaveY(i), j, 0, 0.2, 0, 0, 1, 1);//
                drawNormalLines(i, getWaveY(i), j + 1, 0, 0.2, 0, 0, 1, 1);//
                drawNormalLines(i + 1, getWaveY(i+1), j, 0, 0.2, 0, 0, 1, 1);//
                drawNormalLines(i, getWaveY(i), j + 1, 0, 0.2, 0, 0, 1, 1);//
                drawNormalLines(i + 1, getWaveY(i+1), j, 0, 0.2, 0, 0, 1, 1);
                drawNormalLines(i + 1, getWaveY(i+1), j + 1, 0, 0.2, 0, 0, 1, 1);
            }
        }
    }
}
void RiverBed(void)
{
    float y = 0;
    float totalXgrid = rvr.endXpos+rvr.startXpos; //for river
    float totalZgrid = rvr.startZpos - rvr.endZpos;//for river
    //float riverbedsize = rvr.startXpos;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texA.sandA);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, cr.green);
    for (int i = -rvr.startXpos; i<rvr.startXpos; i+=1) {
        for (float j = -rvr.startZpos; j < -rvr.endZpos; j += 1) {
            glBegin(GL_TRIANGLES);
            glColor3f(1, 1, 0);
            y = -0.5;
            textureGrid(i, y, j, rvr.startXpos, totalXgrid, rvr.startZpos, rvr.endZpos, totalZgrid);
            glEnd();
            normalsForGrids(i, y, j);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
void RiverWalls(){
    float y = 0;
    float depth = 0.5;
    int z = 0;
    float riverbedsize = rvr.startXpos;
    //river vertical walls
    for(int k = 0; k < 2; k++){
        for (int i = -riverbedsize; i< riverbedsize; i++) {
            for (float j = -depth; j < 0; j++) {
                glBegin(GL_TRIANGLES);
                glColor3f(0.5f, 0.35f, 0.05f);
                if(k == 0){
                    y = -rvr.endZpos;
                }else{
                    y = -rvr.startZpos;
                }
                glVertex3f(i, j, y); //1
                glVertex3f(i+1, j, y);//3
                glVertex3f(i,j+0.5,y);//4
                glVertex3f(i+1, j, y);//5
                glVertex3f(i+1,j+0.5, y);//6
                glVertex3f(i,j+0.5,y);//2
                glEnd();
            }
        }
    }
    //river horizontal walls
    for(int k = 0; k < 6; k++){
        for (int i = -14; i<= 14; i++) {
            if(i == -14 || i == 14){
                for (float j = -0.5; j < 0; j++) {
                    glBegin(GL_TRIANGLES);
                    glColor3f(0.5f, 0.35f, 0.05f);
                    if(k == 0){
                        z = -9;
                    }else if (k == 1){
                        z = -10;
                    }else if (k == 2){
                        z = -11;
                    }else if (k == 3){
                        z = -12;
                    }else{
                        z = -13;
                    }
                    glVertex3f(i, j, z); //1
                    glVertex3f(i, j, z+1);//3
                    glVertex3f(i,j+0.5,z);//4
                    glVertex3f(i, j, z+1);//5
                    glVertex3f(i,j+0.5, z+1);//6
                    glVertex3f(i,j+0.5,z);//2
                    glEnd();
                }
            }
        }
    }
}
void grassBforRoad(){
    float y = 0;
    float startP = 5;
    float endP = 10;
    float differP = endP-startP;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texA.groundA);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, cr.green);
    for (int i = -GRIDSIZE; i< GRIDSIZE; i++) {
        for (float j = startP; j < endP; j += 1) {
            glBegin(GL_TRIANGLES);
            glColor3f(1, 1, 0);
            y = 0;//(CYLINDER_RADIUS * 5);
            textureGrid(i, y, j, GRIDSIZE, totalGd, -startP, endP, differP);
            glEnd();
            normalsForGrids(i, y, j);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, texA.groundB);

    float gStartC = 3;
    float gEndC = 5;
    float totalG_C = gEndC+gStartC;
    for (int i = -GRIDSIZE; i< GRIDSIZE; i+=1) {
            //to cover the grid with grass which is not covered by road.
            if(i == -GRIDSIZE || i == -GRIDSIZE+1 && i == GRIDSIZE || i == GRIDSIZE-1){
                for (float j = -gStartC; j < gEndC; j += 1) {
                    glBegin(GL_TRIANGLES);
                    glColor3f(1, 1, 0);
                    y = 0;
                    textureGrid(i, y, j, GRIDSIZE, totalGd, gStartC, gEndC, totalG_C);
                    glEnd();
                    normalsForGrids(i, y, j);
                }
            }
        }
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Road(void){
    float y = 0;
    drawAxes(1);
    glEnable(GL_TEXTURE_2D);
    /*GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat greeni[] = {0.0, 1.0, 0.0, 1.0};*/
    float startP= 3;
    float endP = 5;
    float totalP = endP+startP;
    glBindTexture(GL_TEXTURE_2D, texA.road);
    for (int i = -rvr.startXpos; i< rvr.endXpos; i+=1) {
        for (float j = -startP; j < endP; j += 1) {
            glBegin(GL_TRIANGLES);
            glColor3f(1, 1, 1);
            y = 0;//(CYLINDER_RADIUS * 5);
            textureGrid(i, y, j, GRIDSIZE, totalGd, startP, endP, totalP);
            glEnd();
            normalsForGrids(i, y, j);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void grassAfterRoad(){
    float y = 0;
    float startP = 8;
    float endP = 3;
    float differP = startP-endP;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texA.groundB);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, cr.green);
    for (int i = -GRIDSIZE; i< GRIDSIZE; i+=1) {
        for (float j = -startP; j < -endP; j += 1) {
            glBegin(GL_TRIANGLES);
            glColor3f(1, 1, 0);
            y = 0;//(CYLINDER_RADIUS * 5);
            textureGrid(i, y, j, GRIDSIZE, totalGd, startP, endP, differP);
            glEnd();
            normalsForGrids(i, y, j);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    //grass grid to cover the part river is not filled in with.
    glBindTexture(GL_TEXTURE_2D, texA.groundB);
    float differG = rvr.startZpos-rvr.endZpos;
    for (int i = -GRIDSIZE; i< GRIDSIZE; i+=1) {
        //to cover the grid with grass which is not covered by road.
        if(i == -GRIDSIZE || i == -GRIDSIZE+1 && i == GRIDSIZE || i == GRIDSIZE-1){
            for (float j = -rvr.startZpos; j < -rvr.endZpos; j += 1) {
                glBegin(GL_TRIANGLES);
                glColor3f(1, 1, 0);
                y = 0;
                textureGrid(i, y, j, GRIDSIZE, totalGd, rvr.startZpos, rvr.endZpos, differG);
                glEnd();
                normalsForGrids(i, y, j);
            }
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
void grassAfteRiver(){
    float y = 0;
    float startP = 18;
    float endP = 13;
    float differP = startP-endP;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texA.groundB);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, cr.green);
    for (float i = -GRIDSIZE; i< GRIDSIZE; i+= g.tess) {
         for (float j = -startP; j < -endP; j += g.tess) {
            glBegin(GL_TRIANGLES);
            glColor3f(1, 1, 0);
            y = 0;//(CYLINDER_RADIUS * 5);
            textureGrid(i, y, j,GRIDSIZE, totalGd, startP, endP, differP);
            glEnd();
            normalsForGrids(i, y, j);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
void textureGrid(float i , float y, float j,float gridSize, float total, float startP, float endP, float total_or_differ_P){
   // float y = 0;
    glNormal3f(0, 1, 0);
    glTexCoord2f((gridSize + i) /total, (startP + j) /total_or_differ_P);
    glVertex3f(i, y, j);
    glNormal3f(0, 1, 0);
    glTexCoord2f((gridSize + i) /total, (startP+j+1)/total_or_differ_P);
    glVertex3f(i, y, j + 1);
    glNormal3f(0, 1, 0);
    glTexCoord2f((gridSize + i + 1) / total, (startP + j) /total_or_differ_P);
    glVertex3f(i + 1, y, j);
    glNormal3f(0, 1, 0);
    glTexCoord2f((gridSize + i) / total, (startP+j+1)/total_or_differ_P);
    glVertex3f(i, y, j + 1);
    glNormal3f(0, 1, 0);
    glTexCoord2f((gridSize + i + 1) /total, (startP + j)/total_or_differ_P);
    glVertex3f(i + 1, y, j);
    glNormal3f(0, 1, 0);
    glTexCoord2f((gridSize + i+1) /total, (startP+j+1) / total_or_differ_P);
    glVertex3f(i + 1, y, j+1);
}

void normalsForGrids(float i, float y, float j){
    if (getNormalFlag()){
        drawNormalLines(i, y, j, 0, 0.2, 0, 0, 1, 1);//
        drawNormalLines(i + 1, y, j, 0, 0.2, 0, 0, 1, 1);//
        drawNormalLines(i + 1, y, j + 1, 0, 0.2, 0, 0, 1, 1);
        drawNormalLines(i, y, j + 1, 0, 0.2, 0, 0, 1, 1);//
        drawNormalLines(i, y, j + 1, 0, 0.2, 0, 0, 1, 1);//
        drawNormalLines(i + 1, y, j, 0, 0.2, 0, 0, 1, 1);
    }
}



