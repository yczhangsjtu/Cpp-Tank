#ifndef _MESSAGE_H
#define _MESSAGE_H

enum msgType {msgHitTank, msgHitWall, msgCommand, msgFeedback};

const int cmdAccel = 1; // Accelerate command
const int cmdFire  = 2; // Fire command
const int cmdTankTurn = 3; // Turning tank body
const int cmdTowerTurn = 4; // Turning tank tower

const int fbkNoBullet = 1; // No bullet warn
const int fbkCollide = 2; // Collision warn

typedef struct Message
{
	msgType type;
	    int idx1;
	    int idx2;
	    int i;
		int j;
	  float x;
	  float y;
} Message;

Message createMessage
(
	msgType type, 
	int idx1 = -1, int idx2 = -1,
	int i    = -1, int j    = -1,
	float x = 0.0, float y = 0.0
);

// Emit a piece of message
void emitMessage
(
	msgType type, 
	int idx1 = -1, int idx2 = -1,
	int i    = -1, int j    = -1,
	float x = 0.0, float y = 0.0
);

void emitHitWallMessage(int idx, int i, int j, float x, float y);
void emitHitTankMessage(int ibul, int itank, float x, float y);
void emitCommandMessage(int cmd, int idx, int detail);
void emitFeedbackMessage(int fbk, int idx);

// Deal with messages
void dealWithMessages();
void dealWithMessage(const Message & msg);
void dealWithTankHit(int ibul, int itank, float x, float y);
void dealWithWallHit(int idx, int i, int j, float x, float y);
void dealWithCommand(int cmd, int idx, int accel);
void dealWithFeedBack(int fbk, int idx);

#endif