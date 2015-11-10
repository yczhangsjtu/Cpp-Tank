#ifndef _DRAW_H
#define _DRAW_H

#include "GL\glut.h"
#include "rect.h"
#include "tank.h"

const float wallHeight = wallSize/3;
const float tankHeight = wallHeight/2;
const float eyeHeight  = wallHeight;
const float eyeDist    = 3;
const float mapRatio   = 0.2;
const float scaleNear   = 20;
const float scaleFar    = 2000.0;

enum DrawMode {DRAW_MAP, DRAW_INFO, DRAW3D_1, DRAW3D_2};

void initScene();
void setModelViewMatrix(DrawMode mode);
void setProjectionMatrix(int w, int h, DrawMode mode);

// 2D drawing functions
void drawRect2d(const Rect & rc, float dh = 0.0);
void drawTank2d(const Tank & tank);
void drawTanks2d();
void drawBullet2d(const Bullet & bullet);
void drawBullets2d();
void drawWall2d(int i, int j);
void drawWalls2d();
void drawHeart(GLfloat x, GLfloat y, GLfloat size);
void drawChar(char c, GLfloat x, GLfloat y, GLfloat *color, GLfloat size);
void drawString(GLfloat x, GLfloat y, GLfloat size,
				const char *str, GLfloat *color, int n);
void showHP();
void showBullet();

// 3D drawing functions
void drawRect3d(const Rect & rc, GLfloat height);
void drawGround();
void drawTank3d(const Tank & tank, int idx);
void drawTanks3d();
void drawBullet3d(const Bullet & bullet);
void drawBullets3d();
void drawWall3d(int i, int j);
void drawWalls3d();
void drawEffects();

#endif