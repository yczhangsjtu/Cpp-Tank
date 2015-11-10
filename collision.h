#ifndef _COLLISION_H
#define _COLLISION_H
#include <cstdlib>
// This header file contains all the functions for collision detection

#include "rect.h"

// If line1 and line2 collide with each other
bool lineCollide(const float line1[][2], const float line2[][2], float *p=NULL);
bool lineCollide(const float line11[2], const float line12[2], 
				 const float line21[2], const float line22[2],
				 float *p=NULL);
// If line collide rectangle
bool lineCollideRect(const float line1[2], const float line2[2], const Rect &rc);
// Return true when rc1 and rc2 (considered as empty) collide
bool emptyCollide(const Rect &rc1, const Rect &rc2);
// Return true when rc1 and rc2 (considered as solid) collide
bool solidCollide(const Rect &rc1, const Rect &rc2);
// If a point is contained in a convex polygon
bool inPolygon(const float poly[][2], const float p[], int n);
// If a point is contained in a rect
bool inRect(const Rect & rc, const float p[]);
// If bRc contains sRc
bool containRect(const Rect & bRc, const Rect & sRc);
// Get the visual angle of two points with respect to one point
float visual(const float p1[], const float p2[], const float p[]);

#endif