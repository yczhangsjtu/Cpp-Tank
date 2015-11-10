#include <map>
#include "control.h"
#include "rect.h"
#include "message.h"
#include "tank.h"

extern Rect boundary;
extern std::vector<Message> messages;
extern std::vector<Tank> tanks;
extern int mousex;
extern int mousey;

std::map<int,Command> cmdMap;

void controlTank(int sig)
{
	std::map<int,Command>::const_iterator iter;
	iter = cmdMap.find(sig);
	if(iter!=cmdMap.end())
	{
		int idx = iter->second.idx;
		int code = iter->second.code;
		int commander = iter->second.commander;
		controlTank(idx,code,commander);
	}
}

void controlTank(int idx, int code, int commander)
{
	// Check the validation of this commander to this tank
	if(!isValidCommander(idx, commander))
		return;

	switch(code)
	{
	case turnLeft:
		emitCommandMessage(cmdTankTurn, idx, 1);
		break;
	case turnRight:
		emitCommandMessage(cmdTankTurn, idx, 2);
		break;
	case moveForward:
		emitCommandMessage(cmdAccel, idx, 1);
		break;
	case moveBack:
		emitCommandMessage(cmdAccel, idx, 2);
		break;
	case stopTurning:
		emitCommandMessage(cmdTankTurn, idx, 0);
		break;
	case stopMoving:
		emitCommandMessage(cmdAccel, idx, 0);
		break;
	case towerLeft:
		emitCommandMessage(cmdTowerTurn, idx, 1);
		break;
	case towerRight:
		emitCommandMessage(cmdTowerTurn, idx, 2);
		break;
	case towerStop:
		emitCommandMessage(cmdTowerTurn, idx, 0);
		break;
	case fire:
		emitCommandMessage(cmdFire, idx, 0);
		break;
	default:
		;
	}
}

// Judge if is the right commander
bool isValidCommander(int idx, int commander)
{
	return idx >= 0 && idx < tanks.size()
		   && tanks.at(idx).commander == commander;
}

void addCommandLink(int sig, int idx, int code, int commander)
{
	Command cmd = {idx,code,commander};
	cmdMap[sig] = cmd;
}