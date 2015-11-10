#ifndef _CONTROL_H
#define _CONTROL_H
#include "message.h"
#include <vector>

typedef struct Command
{
	int idx;
	int code;
	int commander;
} Command;

const int moveForward = 0;
const int moveBack    = 1;
const int turnLeft    = 2;
const int turnRight   = 3;
const int stopMoving  = 4;
const int stopTurning = 5;
const int towerLeft   = 6;
const int towerRight  = 7;
const int towerStop   = 8;
const int fire        = 9;

void controlTank(int sig);
void controlTank(int idx, int code, int commander);
bool isValidCommander(int idx, int commander);
void addCommandLink(int sig, int idx, int code, int commander);


#endif