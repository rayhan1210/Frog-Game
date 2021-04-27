#ifndef JUMPING_FUNCTIONALITY_H
#define JUMPING_FUNCTIONALITY_H

#include <stdbool.h>

#define GRAVITY 9.8

bool getJumpingFlag(void);
void jumpingSettings(void);
int jumpingIdle(void);
float calcReach(void);
float calcTimeOfFlight(void);
void calcPosition(float t, float dt);
bool parabolaInsideWindow(void);
void drawDirectionSpeedVector(void);
void drawParabola(void);
void drawParabolaNormalTangent(void);

#endif
