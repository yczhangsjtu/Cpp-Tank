#include <cassert>
#include <cstdio>
#include "tank.h"
#include "collision.h"
#include "rect.h"
#include "message.h"
#include "effects.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

extern std::vector<Tank>   tanks;
extern std::vector<Bullet> bullets;
extern std::vector<Message> messages;
extern std::vector<Effect> effects;
extern int wall[numberHeight][numberWidth];
extern Rect boundary;
extern bool going;

// This file contains the definitions of the functions in Tank.h

// Create functions
Tank createTank(int owner, float x, float y, float angle)
{
	Tank tank = {{{x,y},angle,tankLength,tankWidth},0,fullHP,0,0,
				{angle,fullHP,fullBullet,0},owner};
	return tank;
}

// Add a new tank to the array
bool addTank(int owner, float x, float y, float angle)
{
	Tank tank = createTank(owner,x,y,angle);
	if(!tankCollision(tank.rect))
	{
		tanks.push_back(tank);
		return true;
	}
	return false;
}

bool getFreeTank(Tank & tank)
{
	for(int i = 0; i < 256; i++)
	{
		float x = (float)rand()/RAND_MAX*scaleWidth;
		float y = (float)rand()/RAND_MAX*scaleHeight;
		float a = (float)rand()/RAND_MAX*2*M_PI;
		tank = createTank(-1,x,y,a);
		if(!tankCollision(tank.rect)) return true;
	}
	return false;
}

bool addFreeTank(int owner)
{
	Tank tank;
	if(getFreeTank(tank))
	{
		tank.commander = owner;
		tanks.push_back(tank);
		return true;
	}
	return false;
}

bool reviveTank(int idx)
{
	if(idx < 0 || idx >= tanks.size() || tanks.at(idx).HP > 0) return false;
	Tank tank;
	if(getFreeTank(tank))
	{
		tank.commander = tanks.at(idx).commander;
		tanks.at(idx) = tank;
	}
	return false;
}

// Move functions
void moveon(int dt)
{
	// Move on all the bullets
	if(going)
	{
		for(int i = 0; i < bullets.size(); i++)
			moveonBullet(i, dt);
		// Move on all the tanks
		for(int i = 0; i < tanks.size(); i++)
			moveonTank(i, dt);
		// Deal with messages
		dealWithMessages();
	}
	// Deal with effects
	for(int i = 0; i < effects.size(); i++)
	{
		if(!moveOnEffect(effects.at(i),dt))
		{
			effects.at(i) = effects.back();
			effects.pop_back();
			i--;
		}
	}
}

// Check if rectangle rc is valid on the field
// if idx is valid tank number, then this tank is omitted, this is useful when
// moving a tank
bool tankCollision(const Rect & rc, int idx)
{
	// Collision detection with other tanks
	for(int i = 0; i < tanks.size(); i++)
	{
		if(tanks.at(i).HP <= 0) continue; // Pass dead tank
		if(i == idx) continue; // If this is the tank assigned to omit, pass
		// cld is true only if the two do not collide or contain each other
		if(solidCollide(rc, tanks.at(i).rect)) return true;
	}
	// Collision detection with the walls
	for(int i = 0; i < numberHeight; i++)
	{
		for(int j = 0; j < numberWidth; j++)
		{
			if(wall[i][j]==0) continue; // If there is no wall at (i,j), pass

			Rect wRc = getWallRect(i,j); // Get the rect of the wall
			if(solidCollide(rc, wRc)) return true; // Detect collision
		}
	}
	// Collisiont detection with the boundary
	if(!containRect(boundary,rc)) return true;

	return false;
}

void moveonTank(int idx, int dt)
{
	// Make sure that this function is properly invoked
	assert(idx >=0 && idx < tanks.size());
	if(tanks.at(idx).HP <= 0) return;
	// Deal with the tank body first //////////////////////////////////////////
	// Generate a new rectangle, move the new rectangle, then detect collision
	// if collide, just cancel and return. If no collision, move the tank to
	// the new rectangle
	float ang = tanks.at(idx).rect.angle;
	float spd = tanks.at(idx).speed;
	float dx  = spd * dt * cos(ang); // The displacement of the rectangle
	float dy  = spd * dt * sin(ang); // The displacement of the rectangle
	Rect nRc = tanks.at(idx).rect; // create new rectangle
	nRc.pos[0] += dx; // Move the new rectangle
	nRc.pos[1] += dy;

	if(tanks.at(idx).turning == 1)
	{
		nRc.angle += tankTurnSpeed * dt; // Rotate the new rectangle
		if(nRc.angle > 2*M_PI) nRc.angle -= 2*M_PI;
		tanks.at(idx).tower.angle += tankTurnSpeed * dt;
		if(tanks.at(idx).tower.angle > 2*M_PI)
			tanks.at(idx).tower.angle -= 2*M_PI;
	}
	else if(tanks.at(idx).turning == 2)
	{
		nRc.angle  -= tankTurnSpeed * dt; // Rotate the new rectangle
		if(nRc.angle < 0) nRc.angle += 2*M_PI;
		tanks.at(idx).tower.angle -= tankTurnSpeed * dt;
		if(tanks.at(idx).tower.angle < 0)
			tanks.at(idx).tower.angle += 2*M_PI;
	}

	// Collision detection
	bool cld = tankCollision(nRc, idx);

	if(cld) // If there is collision
	{
		tanks.at(idx).speed = -tanks.at(idx).speed * rebRatio; // Rebounce
		tanks.at(idx).turning = 0;
		emitFeedbackMessage(fbkCollide, idx);
	}
	else // If there is no collision
	{
		tanks.at(idx).rect = nRc; // Move the tank to the new rectangle
		// If accelerating
		if(tanks.at(idx).accel == 1)
		{
			tanks.at(idx).speed += tankAccel * dt;
		}
		else if(tanks.at(idx).accel == 2)
		{
			tanks.at(idx).speed -= tankAccel * dt;
		}
		// Slow down by friction
		if(tanks.at(idx).speed > 0)
		{
			tanks.at(idx).speed -= friction * dt * tanks.at(idx).speed;
			if(tanks.at(idx).speed < 0) tanks.at(idx).speed = 0;
		}
		else if(tanks.at(idx).speed < 0)
		{
			tanks.at(idx).speed -= friction * dt * tanks.at(idx).speed;
			if(tanks.at(idx).speed > 0) tanks.at(idx).speed = 0;
		}
	}
	////////////////Done with the body movement////////////////////////////////

	// Then deal with the tower (there is no need for detection) //////////////
	switch(tanks.at(idx).tower.turning)
	{
	case 1: // Turning left until maximum angle
		tanks.at(idx).tower.angle += towerTurnSpeed * dt;
		if(tanks.at(idx).tower.angle > 2*M_PI)
			tanks.at(idx).tower.angle -= 2*M_PI;
		break;
	case 2: // Turning right until maximum angle
		tanks.at(idx).tower.angle -= towerTurnSpeed * dt;
		if(tanks.at(idx).tower.angle < 0)
			tanks.at(idx).tower.angle += 2*M_PI;
		break;
	}
}

void moveonBullet(int idx, int dt)
{
	// Make sure that this function is invoked properly
	assert(idx >=0 && idx < bullets.size());

	float *p = bullets.at(idx).pos;
	float a  = bullets.at(idx).angle;
	float v  = bullets.at(idx).speed;

	// Detect Collsion first
	bool hit = false;
	// Collision detection with the wall
	for(int i = 0; i < numberHeight; i++)
	{
		for(int j = 0; j < numberWidth; j++)
		{
			if(wall[i][j]==0) continue; // If there is no wall, pass

			Rect wRc = getWallRect(i,j); // Else, get the rectangle of this wall
			if(inRect(wRc, p)) // If the bullet is in the wall
			{
				hit = true; // Hit
				// Calculate the point where the bullet entered the wall
				float trace[][2] = {
					{p[0],p[1]}, {p[0]-10*v*dt*cos(a),p[1]-10*v*dt*sin(a)}
				};
				float wallV[4][2];
				float inter[2];
				getVertices(wRc,wallV); // Get the vertices of the wall
				bool c =
				lineCollide(wallV[0],wallV[1],trace[0],trace[1],inter) ||
				lineCollide(wallV[1],wallV[2],trace[0],trace[1],inter) ||
				lineCollide(wallV[2],wallV[3],trace[0],trace[1],inter) ||
				lineCollide(wallV[3],wallV[0],trace[0],trace[1],inter);
				// Emit the hit message
				if(c)
					emitHitWallMessage(idx, i, j, inter[0], inter[1]); 
				else
					emitHitWallMessage(idx, i, j, p[0], p[1]); 
			}
			if(hit) break; // If already hit, break the loop
		}
		if(hit) break;
	}
	// Collision with tanks
	for(int i = 0; i < tanks.size(); i++)
	{
		if(tanks.at(i).HP <= 0) continue;
		Rect tRc = tanks.at(i).rect;
		if(inRect(tRc, p))
		{
			hit = true;
			emitHitTankMessage(idx, i, p[0], p[1]); // Emit the hit message
			break; // Break the loop
		}
	}
	// Collision with boundary, treated as hitting wall
	if(!inRect(boundary, p)) emitHitWallMessage(idx, -1, -1, p[0], p[1]);

	// Move the bullet
	p[0] += v * dt * cos(a);
	p[1] += v * dt * sin(a);
}

// Get the rectangle of the wall at position (i,j)
Rect getWallRect(int i, int j)
{
	float x = j * wallSize + 0.5 * wallSize;
	float y = i * wallSize + 0.5 * wallSize;
	Rect rc = {{x,y},0,wallSize,wallSize};
	return rc;
}


//////////////////////////////Tank.cpp/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
