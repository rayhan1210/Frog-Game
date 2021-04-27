#ifndef GRID_PLANE_H
#define GRID_PLANE_H

#define GENERATELENGTH 25
#define CRADIUS 0.1
#define CHEIGHT 1
#define CUBE_LENGTH 1
#define GRIDSIZE 15
#define CSLICES 8


typedef struct{
    float startXpos;
    float startZpos;
    float endXpos;
    float endZpos;
    float A ;
    float k ;
    float wave;


}RoadAndRiver;
static RoadAndRiver rvr = {
    14.0, 13.0, 14.0, 8.0,
    0.08, (2 * 3.14) / 10, 0
};
typedef struct {


}road;
typedef struct {
    float tess;
}global;
static global g = {1.0};

typedef struct{
    GLuint road;
    GLuint groundA;
    GLuint groundB;
    GLuint soilA;
    GLuint sandA;
}texture;
static texture texA = {
    0 , 0, 0
};

typedef struct {
    float greenA;
    float redA;
    float blueA;
}color3f;
static const color3f greenA ={0.3, 1, 0.3};
static const color3f redA =    {1.0, 0.0, 0.0};
static const color3f blueA =    {0.0, 0.0, 1.0};

void drawAxes(float scale);
void initCylinder(void);
void drawCylinder(void);
void waterGrid();
void RiverBed(void);
void RiverWalls();
void grassAfteRiver();
float getWaveY(float x);
void initTextures();
void drawNormalLines(float x, float y, float z, float i, float j, float k, float a, float b, float c);
static GLuint loadTexture(const char *filename);
void init_sphere(int stacks, int slices);//?
void grassBforRoad();
void Road(void);
void grassAfterRoad();
void normalsForGrids(float i, float y, float j);
void textureGrid(float i , float y, float j,float gridSize, float total, float startP, float endP, float total_or_differ_P);
#endif


