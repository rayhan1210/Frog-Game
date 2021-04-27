#ifndef PROCEDURAL_H
#define PROCEDURAL_H


#define OBJGENERATION(min, max) ((rand()%(int)((max+1)-min)) + min)
#define CARSPEED 0.004
#define LOGSPEED 0.004

void initCars(void);
void initLogs(void);
void drawCars(void);
void drawLogs(void);
void updateCars(void);
void updateLogs(void);
void drawcube(double x,double y, double z);
void func(double x, double y, double z);
void draw_cylinder(GLfloat radius, GLfloat height);
void drawMainCarCubes(void);
void drawSphere(void);
void drawFrogLeg(void);

#endif


