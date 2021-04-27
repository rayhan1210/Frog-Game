#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include <stdbool.h>

#define REDUCTION 25
#ifndef M_PI
#define M_PI 3.14159265359
#endif
#define UNUSED_VAR (void)

typedef struct{
	float x, y, z;
	float r, theta, phi;
	float dx, dy, dz;
} projection;

typedef struct {
	projection r0, r;
	bool onLog;
	int whichLog;
	bool dead;
} frogState;

typedef struct {
	float x, y, z;
} matrix;

void setProjectionMatrix(void);
void setupCamera(void);
void setSegments(int newSegments);
void setWidth(float newWidth);
void setHeight(float newHeight);
void setRotateCamX(float newRotateCamX);
void setRotateCamY(float newRotateCamY);
void setCamZoom(float newCamZoom);
void updateCartesian(projection *p);
void updateSpherical(projection *p);
void switchDebug(void);
void switchCartesianFlag(void);
void switchTangentFlag(void);
void switchNormalFlag(void);
void switchWireFlag(void);
void switchAxesFlag(void);
void setScore(int a);
void setLives(int a);
int getSegments(void);
int getScore(void);
int getLives(void);
float getWidth(void);
float getHeight(void);
float getRotateCamX(void);
float getRotateCamY(void);
float getCamZoom(void);
bool getDebug(void);
bool getCartesianFlag(void);
bool getTangentFlag(void);
bool getNormalFlag(void);
bool getWireFlag(void);
bool getAxesFlag(void);

extern frogState frog;
extern matrix cars[32];
extern matrix logs[40];

#endif
