#include <cmath>
#include <cstdio>
#include "rect.h"

// Create a rectangle
Rect createRect(float x, float y, float angle, float width, float height)
{
	Rect rc = {{x,y},angle,width,height};
	return rc;
}

// Get the vertices of a rectangle. store them into an array (4*2)
void getVertices(const Rect & rc, float vtc[][2])
{
	float a = rc.angle;
	float x = rc.pos[0], y = rc.pos[1];
	float w = rc.width,  h = rc.height;

	vtc[0][0] = x + w/2 * cos(a) - h/2 * sin(a);
	vtc[0][1] = y + h/2 * cos(a) + w/2 * sin(a);
	vtc[1][0] = x - w/2 * cos(a) - h/2 * sin(a);
	vtc[1][1] = y + h/2 * cos(a) - w/2 * sin(a);
	vtc[2][0] = x - w/2 * cos(a) + h/2 * sin(a);
	vtc[2][1] = y - h/2 * cos(a) - w/2 * sin(a);
	vtc[3][0] = x + w/2 * cos(a) + h/2 * sin(a);
	vtc[3][1] = y - h/2 * cos(a) + w/2 * sin(a);
}

// Move the rectangle
void moveAlongHeight(Rect & rc, float dist)
{
	rc.pos[0] -= dist * sin(rc.angle);
	rc.pos[1] += dist * cos(rc.angle);
}
void moveAlongWidth(Rect & rc, float dist)
{
	rc.pos[0] += dist * cos(rc.angle);
	rc.pos[1] += dist * sin(rc.angle);
}
