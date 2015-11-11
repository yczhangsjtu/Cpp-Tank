#include <cmath>
#include <cstdio>
#include "collision.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif


// Judge whether two line pieces intersect
// The algorithm is to get the intersect of the two infinite lines, and judge
// whether that intersect lie in both of the line pieces
// Put the intersect at p
bool lineCollide(const float line1[][2], const float line2[][2], float *p)
{
	float x1=line1[0][0], y1=line1[0][1], x2=line1[1][0], y2=line1[1][1];
	float x3=line2[0][0], y3=line2[0][1], x4=line2[1][0], y4=line2[1][1];

	// Parallel
	float xd = (x1*(x4*(-y2+y3)+x3*(y2-y4))+x2*(x4*(y1-y3)+x3*(-y1+y4)));
	float yd = -(((x2*y1-x1*y2)*(y3-y4)+(y1-y2)*(-x4*y3+x3*y4)));
	float d  = (x4-x3)*(y1-y2)+(x1-x2)*(y3-y4);

	if(d==0) return false;

	// The intersect
	float x = xd/d;
	float y = yd/d;
	if(p)
	{
		p[0] = x;
		p[1] = y;
	}
	// Judge if the intersect lies on both of the line pieces
	if(x1!=x2)
	{
		if(x<=x1 && x<=x2) return false;
		if(x>=x1 && x>=x2) return false;
	}
	else
	{
		if(y<=y1 && y<=y2) return false;
		if(y>=y1 && y>=y2) return false;
	}
	if(x3!=x4)
	{
		if(x<=x3 && x<=x4) return false;
		if(x>=x3 && x>=x4) return false;
	}
	else
	{
		if(y<=y3 && y<=y4) return false;
		if(y>=y3 && y>=y4) return false;
	}
	return true;
}

bool lineCollide(const float line11[2], const float line12[2], 
				 const float line21[2], const float line22[2],
				 float *p)
{
	float line1[2][2] = {{line11[0],line11[1]},{line12[0],line12[1]}};
	float line2[2][2] = {{line21[0],line21[1]},{line22[0],line22[1]}};
	return lineCollide(line1,line2,p);
}

bool lineCollideRect(const float line1[2], const float line2[2], const Rect &rc)
{
	float vtc[4][2];
	getVertices(rc,vtc);
	return  lineCollide(line1,line2,vtc[0],vtc[1]) ||
			lineCollide(line1,line2,vtc[1],vtc[2]) ||
			lineCollide(line1,line2,vtc[2],vtc[3]) ||
			lineCollide(line1,line2,vtc[3],vtc[0]);
}

// Return true when rc1 and rc2 (considered as empty) collide
bool emptyCollide(const Rect &rc1, const Rect &rc2)
{
	// First, check each line piece in rc1 and each in rc2 to see
	// whether there are collision
	float v1[5][2], v2[5][2];
	// Get the vertices' coordinates
	getVertices(rc1,v1);
	getVertices(rc2,v2);
	// Add one more vertex to form a loop
	v1[4][0] = v1[0][0]; v1[4][1] = v1[0][1];
	v2[4][0] = v2[0][0]; v2[4][1] = v2[0][1];
	// Loop to check
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(lineCollide(v1+i,v2+j)) return true;
	return false;
}

// Return true when rc1 and rc2 (considered as solid) collide
bool solidCollide(const Rect &rc1, const Rect &rc2)
{
	// First, check whether there are line collision
	if(emptyCollide(rc1,rc2)) return true;

	// Second, check if one contain the other
	// Since by now we've already known that they do not collide
	// it suffices to check if the center of one rectangle is in the other
	if(inRect(rc1,rc2.pos)) return true;
	if(inRect(rc2,rc1.pos)) return true;

	return false;
}


// If a point is contained in a convex polygon
// this is equivalent to that the sum of all the angles(centered at p) is 2*Pi
bool inPolygon(const float poly[][2], const float p[], int n)
{
	float sum = 0.0, tol = 0.01;
	for(int i = 0; i < n-1; i++)
		sum += visual(poly[i],poly[i+1],p);
	sum += visual(poly[n-1],poly[0],p);
	return fabs(sum - 2*M_PI) < tol;
}


// If a point is contained in a rect
bool inRect(const Rect & rc, const float p[])
{
	float v[4][2];
	getVertices(rc,v);
	return inPolygon(v,p,4);
}

// If bRc contains sRc
bool containRect(const Rect & bRc, const Rect & sRc)
{
	if(emptyCollide(bRc,sRc)) return false; // If the boundaries collide, false
	if(!inRect(bRc,sRc.pos)) return false; // If bRc does not contain center of sRc
	return true;
}

// Get the visual angle of two points with respect to one point
// Use the cosine theorem
float visual(const float p1[], const float p2[], const float p[])
{
	float aa = (p1[0]-p[0])*(p1[0]-p[0])+(p1[1]-p[1])*(p1[1]-p[1]);
	float bb = (p2[0]-p[0])*(p2[0]-p[0])+(p2[1]-p[1])*(p2[1]-p[1]);
	float cc = (p2[0]-p1[0])*(p2[0]-p1[0])+(p2[1]-p1[1])*(p2[1]-p1[1]);
	float a  = sqrt(aa);
	float b  = sqrt(bb);
	return acos((aa+bb-cc)/(2*a*b));
}
