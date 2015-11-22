#ifndef _TANK_H
#define _TANK_H

#include <vector> // Used to store all the objects
#include "rect.h"
#include "message.h"
// This header file defines all the environment variables and structures of this project
// and all the functions to manipulate them

///////////////////////////////////////////////////////////////////////////////
// Definition of all the structures ///////////////////////////////////////////

typedef struct Tower
{
	float angle;  // Angle of this tower (with respect to the absolute space, not the tank body)
	int   HP;     // Health value
	int   bullet; // Number of bullets remained
	int   turning;// If this tower is chaning direction. 1 means left, 2 means right, others mean no
} Tower;

typedef struct Tank
{
	Rect  rect;   // Area this tank takes
	float speed;  // Speed of this tank
	int   HP;     // Health value
	int   turning;// If this tank is changing direction, 0 means no, 1 means left, 2 means right
	int   accel;  // If this tank is accelerating, 0 means no, 1 means yes, -1 means decelerating
	Tower tower;  // Tower of this tank
	int   commander;  // Commander of this tank
} Tank;

typedef struct Bullet
{
	float pos[2]; // Position of this bullet
	float angle;  // Direction of this bullet
	float speed;  // Speed of this bullet
} Bullet;

///////////////////////////////////////////////////////////////////////////////
// Here are all the constants//////////////////////////////////////////////////
// Sizes
const float tankWidth  = 40.0;
const float tankLength = 60.0;

const float wallSize   = 100.0;

const int   numberWidth  = 10;
const int   numberHeight = 10;

const float scaleWidth  = numberWidth * wallSize;
const float scaleHeight = numberHeight * wallSize;
const float whRatio     = 1.0;
// Speeds
const float tankAccel      = 0.00005; // Acceleration of each tank
const float origBullSpeed  = 0.05; // Original speed of each bullet when created
const float towerTurnSpeed = 0.00001; // Turning speed of tower
const float tankTurnSpeed  = 0.00001; // Turning speed of tank body
const float friction       = 0.006; // Friction of the ground

// Others
const int fullHP = 3;
const int fullBullet = 1000;
const float rebRatio = 0.3;


///////////////////////////////////////////////////////////////////////////////
// Functions //////////////////////////////////////////////////////////////////

// Create new tank
Tank createTank(int owner, float x, float y, float angle);
bool addTank(int owner, float x, float y, float angle); // return true if success
bool getFreeTank(Tank & tank);
bool addFreeTank(int owner);
bool reviveTank(int idx);
// Move on the whole environment by a short time
void moveon(int dt);
bool tankCollision(const Rect & rc, int idx = -1);
void moveonTank(int idx, int dt);
void moveonBullet(int idx, int dt);

// Other functions
// Get the rectangle of the wall at position (i,j)
Rect getWallRect(int i, int j);


#endif
