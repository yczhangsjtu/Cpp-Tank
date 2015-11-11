#include "message.h"
#include "tank.h"
#include "effects.h"
#include "draw.h"
#include <vector>

extern std::vector<Message> messages;
extern std::vector<Tank>   tanks;
extern std::vector<Bullet> bullets;
extern int wall[numberHeight][numberWidth];

Message createMessage
(
	msgType type, 
	int idx1, int idx2,
	int    i, int    j,
	float  x, float  y
)
{
	Message msg = {type, idx1, idx2, i, j, x, y};
	return msg;
}

// Emit messages

void emitMessage
(
	msgType type, 
	int idx1, int idx2,
	int    i, int    j,
	float  x, float  y
)
{
	messages.push_back(createMessage(type,idx1,idx2,i,j,x,y));
}

void emitHitWallMessage(int idx, int i, int j, float x, float y)
{
	emitMessage(msgHitWall, idx, -1, i, j, x, y);
}

void emitHitTankMessage(int ibul, int itank, float x, float y)
{
	emitMessage(msgHitTank, ibul, itank, -1, -1, x, y);
}

void emitCommandMessage(int cmd, int idx, int detail)
{
	emitMessage(msgCommand, cmd, idx, detail);
}

void emitFeedbackMessage(int fbk, int idx)
{
	emitMessage(msgFeedback, fbk, idx);
}

/////////////////////////////////////////////////////

// Deal with messages
void dealWithMessages()
{
	for(int i = 0; i < messages.size(); i++)
		dealWithMessage(messages.at(i));
	messages.clear();
}

void dealWithMessage(const Message & msg)
{
	switch(msg.type)
	{
	case msgHitTank:
		dealWithTankHit(msg.idx1, msg.idx2, msg.x, msg.y);
		break;
	case msgHitWall:
		dealWithWallHit(msg.idx1, msg.i, msg.j, msg.x, msg.y);
		break;
	case msgCommand:
		dealWithCommand(msg.idx1, msg.idx2, msg.i);
		break;
	case msgFeedback:
		dealWithFeedBack(msg.idx1, msg.idx2);
		break;
	default:
		;
	}
}

// The message of a bullet hitting a tank
void dealWithTankHit(int ibul, int itank, float x, float y)
{
	if(	ibul >= 0 && ibul < bullets.size() &&
		itank>= 0 && itank< tanks.size())
	{
		if(tanks.at(itank).HP > 0)
		{
			tanks.at(itank).HP --;
			if(tanks.at(itank).HP > 0)
			{
				addShineEffect(itank);
				addExplosionEffect(x,y,tankHeight*2.0/3.0);
			}
			else
				addExplosionEffect(x,y,0.0);
			
		}
		bullets.at(ibul) = bullets.back();
		bullets.pop_back();
	}
	addExplosionEffect(x,y,tankHeight);
}

// The message of a bullet hitting a wall
void dealWithWallHit(int idx, int i, int j, float x, float y)
{
	if(idx >= 0 && idx < bullets.size())
	{
		if(i < 0 || j < 0 || i >= numberHeight || j >= numberWidth)
		{
			addExplosionEffect(x,y,tankHeight*2.0/3.0);
		}
		else
		{
			if(wall[i][j]>0 && wall[i][j]<4) wall[i][j]--;
			if(wall[i][j]>0)
				addExplosionEffect(x,y,tankHeight*2.0/3.0);
			else
				addExplosionEffect(x,y,0.0);
		}
		// Remove this bullet
		bullets.at(idx) = bullets.back();
		bullets.pop_back();
	}
}

// The message of a command
void dealWithCommand(int cmd, int idx, int detail)
{
	if(tanks.at(idx).HP <= 0) return;
	if(idx >= 0 && idx < tanks.size())
	{
		Tank & tank = tanks.at(idx);
		switch(cmd)
		{
		case cmdAccel:
			{
				tank.accel = detail;
			}
			break;
		case cmdTankTurn:
			{
				tank.turning = detail;
			}
			break;
		case cmdTowerTurn:
			{
				tank.tower.turning = detail;
			}
			break;
		case cmdFire:
			{
				int & b = tank.tower.bullet;
				if(b > 0)
				{
					// Compute the parameters for the bullet
					float a = tank.tower.angle;
					float x = tank.rect.pos[0], y = tank.rect.pos[1];
					x += tankLength*cos(a);
					y += tankLength*sin(a);
					Bullet bul = {{x,y},a,origBullSpeed};
					bullets.push_back(bul);
					tank.speed -= 0.1*cos(tank.tower.angle-tank.rect.angle);
					b --;
				}
				else
				{
					// Warn the bullet insufficient message
					emitFeedbackMessage(fbkNoBullet,idx); 
				}
			}
			break;
		default:
			;
		}
	}
}

// The message of a feedback from the program
void dealWithFeedBack(int fbk, int idx)
{

}
