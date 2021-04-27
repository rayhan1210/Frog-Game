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

#include "jumpingFunctionality.h"
#include "camera.h"

void calcPositionNumerical(float dt);
bool collisionDetection(void);

static float startTime = 0;

static bool jumpingFlag = false;

void drawCartesianParabola(void);
void drawParametricParabola(void);

void drawDirectionSpeedVector(void)
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 1);
	if (getJumpingFlag())
	{
		glVertex3f(frog.r.x, frog.r.y, frog.r.z);
		glVertex3f(frog.r.dx*0.1 + frog.r.x, frog.r.dy*0.1 + frog.r.y,
				frog.r.dz*0.1 + frog.r.z);
	}
	else
	{
		glVertex3f(frog.r0.x, frog.r0.y, frog.r0.z);
		glVertex3f(frog.r0.dx*0.1 + frog.r0.x, frog.r0.dy*0.1 + frog.r0.y,
				frog.r0.dz*0.1 + frog.r0.z);
	}
	glEnd();
}

void drawParabola(void)
{
	/* Tha parabola change color to red when the frog will land outside */
	if (parabolaInsideWindow())
		glColor3f(0, 0, 1);
	else
		glColor3f(1, 0, 0);

	drawParametricParabola();
}

void drawParabolaNormalTangent(void)
{
	float x, y, z, dx, dy, dz, n;
	for (int i = 0; i <= getSegments(); i++)
	{
		float t = (i/(float)getSegments()) * calcTimeOfFlight();
		x = t * frog.r0.dx;
		z = t * frog.r0.dz;
		y = t * frog.r0.dy - (GRAVITY*t*t)/2;
		dx = frog.r0.dx;
		dz = frog.r0.dz;
		dy = frog.r0.dy - (GRAVITY*t);

		if (getTangentFlag())
		{
			glBegin(GL_LINES);
			glColor3f (0, 1, 1);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z + z);
			n = sqrt(dx*dx + dy*dy + dz*dz)*REDUCTION;
			glVertex3f(frog.r0.x + dx/n + x, frog.r0.y + dy/n + y, frog.r0.z + dz/n + z);

			glEnd();
		}
	}

}

void drawParametricParabola(void)
{
	glBegin(GL_LINE_STRIP);

	float ToF = calcTimeOfFlight();
	for (int i = 0; i <= getSegments(); i++)
	{
		float t = (i/(float)getSegments())*ToF;
		float x = t * frog.r0.dx;
		float z = t * frog.r0.dz;
		float y = t * frog.r0.dy - (GRAVITY*t*t)/2;
		glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z + z);
	}
	glEnd();
}

bool getJumpingFlag(void)
{
	return jumpingFlag;
}

/*
 * Function executed when space key is pressed,
 * and just allow jumping inside the window
 */
void jumpingSettings(void)
{
	if (!jumpingFlag && parabolaInsideWindow())
	{
		jumpingFlag = true;
		startTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		frog.r.theta = frog.r0.theta;
		frog.r.r = frog.r0.r;
		frog.r.phi = frog.r0.phi;
		updateCartesian(&frog.r);
		glutPostRedisplay();
	}
}

int jumpingIdle(void)
{
	static float tLast = -1.0;
	float t, dt;

	if (!jumpingFlag)
		return -1;

	t = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - startTime;

	if (tLast < 0.0)
	{
		tLast = t;
		return -1;
	}

	dt = t - tLast;

	calcPosition(t, dt);

	if (!jumpingFlag)
		tLast = -1;
	else
		tLast = t;

	return 0;
}

/* Function responsible for calculating how far the frog will go */
float calcReach(void)
{
	return ((frog.r0.r*frog.r0.r) / GRAVITY) * sin(2*frog.r0.theta);
}

/* Function responsible for calculating how far the frog will go */
float calcTimeOfFlight(void)
{
	return (2*(frog.r0.r * cos(frog.r0.theta)))/(GRAVITY);
}

void calcPosition(float t, float dt)
{
	UNUSED_VAR t;

	calcPositionNumerical(dt);
}

/* Using the calcReach, checks if will land inside the window */
bool parabolaInsideWindow(void)
{
	//printf("%f,%f\n", calcReach());
		float reachX = frog.r0.x + calcReach() * sin(frog.r0.phi);
		float reachZ = frog.r0.z + calcReach() * cos(frog.r0.phi);
		//printf("%f,%f\n", reachX, reachZ);
        //make sure it doesnt exceed the terrain grid
		if ((reachX > 15 || reachX < -15) || (reachZ > 10 || reachZ < -20))
			return false;

		return true;
}

bool logDetection(void){
	for (int i = 0; i < 40; i++)
		{
			if ((frog.r.x < logs[i].x + 1.35) && (frog.r.x > logs[i].x) && (frog.r.y < 0.1) && (frog.r.z > logs[i].z - 0.1) && (frog.r.z < logs[i].z + 0.1))
			{
				frog.onLog = true;
				frog.whichLog = i;
				//printf("On Log %i\n", i);
				return true;
			}
	}
	frog.onLog = false;
	return false;
}

bool carDetection(void){
	for (int i = 0; i < 32; i++)
		{
			if((frog.r.x < cars[i].x + 0.9) && (frog.r.x > cars[i].x) && (frog.r.y < 0.1) &&
                    (frog.r.z > cars[i].z - 0.5) && (frog.r.z < cars[i].z + 0.5)){
				//printf("On Car %i\n", i);
				return true;
			}
	}
	return false;
}

void calcPositionNumerical(float dt)
{
	frog.r.x += frog.r.dx * dt;
	frog.r.y += frog.r.dy * dt;
	frog.r.z += frog.r.dz * dt;

	frog.r.dy += -GRAVITY * dt;
	updateSpherical(&frog.r);

	if (logDetection())
	{
		jumpingFlag = false;
		frog.r0.x = frog.r.x;
		frog.r0.y = frog.r.y;
		frog.r0.z = frog.r.z;
		updateSpherical(&frog.r0);
		frog.r = frog.r0;
		glutPostRedisplay();
	}else if (carDetection()){
		if (getLives() == 0){
			frog.dead = true;
			return;
		}else{
			setLives(getLives() - 1);
			jumpingFlag = false;
			frog.r.x = frog.r0.x = 0;
			frog.r.y = frog.r0.y = 0;
			frog.r.z = frog.r0.z = 8;
		}
	}else if (frog.r.y < 0.0){
		//printf("%f,%f,%f\n", frog.r.x, frog.r.y, frog.r.z);
		if (frog.r.z < -7.8){
			if(frog.r.z < -9.2){
				setScore(getScore() + 100);
				jumpingFlag = false;
				frog.r.x = frog.r0.x = 0;
				frog.r.y = frog.r0.y = 0;
				frog.r.z = frog.r0.z = 8;
				return;
			}else{
				if (getLives() == 0){
					frog.dead = true;
					return;
				}else{
					setLives(getLives() - 1);
					if(getScore() >= 25 ){
						setScore(getScore() - 25);
					}
					jumpingFlag = false;
					frog.r.x = frog.r0.x = 0;
					frog.r.y = frog.r0.y = 0;
					frog.r.z = frog.r0.z = 8;
					return;
				}
			}
		}
		frog.r.y = 0;
		jumpingFlag = false;
		frog.r0.x = frog.r.x;
		frog.r0.y = frog.r.y;
		frog.r0.z = frog.r.z;
		updateSpherical(&frog.r0);
		frog.r = frog.r0;
		glutPostRedisplay();
	}

}
