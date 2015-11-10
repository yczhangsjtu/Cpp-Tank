#include <cstdlib>
//#include <iostream>
#include <cmath>
#include "ai.h"
#include "tank.h"
#include "control.h"
#include "collision.h"
#include "main.h"

extern int me;
extern std::vector<Tank>   tanks;
extern int wall[numberHeight][numberWidth];
extern Rect boundary;

int Probability[] = {
	2, // turnLeft
	2, // turnRight
	8, // moveForward
	1, // moveBack
	8, // stopTurning
	3, // stopMoving
	0, // towerLeft
	0, // towerRight
	0, // towerStop
	0 // fire
};

/*
* Establish control link
* The higher 8 bits is the command, and the lower 8 bits is the tank index
*/
void initAI()
{
	for(int i = 0; i <= maxControl; i++)
	{
		addCommandLink((turnLeft   <<8)|i,i,turnLeft   ,2);
		addCommandLink((turnRight  <<8)|i,i,turnRight  ,2);
		addCommandLink((moveForward<<8)|i,i,moveForward,2);
		addCommandLink((moveBack   <<8)|i,i,moveBack   ,2);
		addCommandLink((stopTurning<<8)|i,i,stopTurning,2);
		addCommandLink((stopMoving <<8)|i,i,stopMoving ,2);
		addCommandLink((towerLeft  <<8)|i,i,towerLeft  ,2);
		addCommandLink((towerRight <<8)|i,i,towerRight ,2);
		addCommandLink((towerStop  <<8)|i,i,towerStop  ,2);
		addCommandLink((fire       <<8)|i,i,fire       ,2);
	}
}

/*
* Make decision totally randomly
*/
void aiRandom(int idx)
{
	int sum = 0;
	for(int i = 0; i < 10; i++)
		sum += Probability[i];
	int p = rand() % sum;
	sum = 0;
	for(int cmd = 0; cmd < 10; cmd++)
	{
		sum += Probability[cmd];
		if(sum > p)
		{
			controlTank((cmd<<8)|idx);
			return;
		}
	}
	
}

/*
* Move randomly, and when seeing target(of specific commander), turn to it
* and fire
*/
bool aiAutoAim(int idx, int targ)
{
	// Find enemies in sight
	float *p  = tanks.at(idx).rect.pos;
	for(int i = 0; i < tanks.size(); i++)
	{
		if(i==idx) continue;
		if(tanks.at(i).commander == targ)
		{
			bool blocked = false;
			float *tp = tanks.at(i).rect.pos;
			for(int j = 0; j < numberHeight; j++)
			{
				for(int k = 0; k < numberWidth; k++)
				{
					if(wall[j][k] > 0 && lineCollideRect(p,tp,getWallRect(j,k)))
					{
						blocked = true;
						break;
					}
				}
				if(blocked) break;
			}
			if(!blocked) // If the enemy is in sight
			{
				float ta; // target angle
				float a = tanks.at(idx).tower.angle;
				// Calculate the target angle
				float dx = tp[0] - p[0];
				float dy = tp[1] - p[1];
				if(dx==0.0)
				{
					if(dy > 0.0) // Right above
						ta = M_PI/2.0;
					else // Right under
						ta = 3.0*M_PI/2.0;
				}
				else
				{
					ta = atan(dy/dx);
					if(dx > 0.0)
					{
						if(ta < 0.0) ta+=2*M_PI;
					}
					else
					{
						ta+=M_PI;
					}
				}
				controlTank((stopTurning<<8)|idx);
				if(fabs(ta-a) < tankTurnSpeed*aiInterval)
				{
					controlTank((towerStop<<8)|idx);
					if(rand() % 10 > 8)
						controlTank((fire<<8)|idx);
					return true;
				}
				else if((ta > a && fabs(ta-a)<M_PI) ||
						(ta < a && fabs(ta-a)>M_PI))
				{
					controlTank((towerLeft<<8)|idx);
					return true;
				}
				else
				{
					controlTank((towerRight<<8)|idx);
					return true;
				}
			}
		}
	}
	return false;
}