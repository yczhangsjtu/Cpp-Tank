#ifndef _RECT_H
#define _RECT_H

#include <cmath>

typedef struct Rect
{
	float pos[2]; // Position of this rectangle (coordinates of the center)
	float angle;  // Direction of this rectangle
	float width;  // Width of this rectangle
	float height; // Height of this rectangle
} Rect;

// Create a rectangle
Rect createRect(float x, float y, float angle, float width, float height);
// Get the vertices of a rectangle. store them into an array (4*2)
void getVertices(const Rect & rc, float vtc[][2]);
// Move the rectangle
void moveAlongWidth(Rect & rc, float dist);
void moveAlongHeight(Rect & rc, float dist);

#endif