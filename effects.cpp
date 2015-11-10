#include <vector>
#include "effects.h"

extern std::vector<Effect>  effects;

void addEffect(effecType type, float x, float y, float z, int i, int j, int k)
{
	Effect effect = {type,x,y,z,i,j,k};
	effects.push_back(effect);
}

void addShineEffect(int i)
{
	addEffect(effShine,0.0,0.0,0.0,i,1000,0);
}

void addFlickerEffect(int i)
{
	addEffect(effFlicker,0.0,0.0,0.0,i,1000,0);
}

void addExplosionEffect(float x, float y, float z)
{
	addEffect(effExplosion,x,y,z,0,500,0);
}

bool moveOnEffect(Effect & effect, int dt)
{
	switch(effect.type)
	{
	case effShine:
	{
		effect.j -= dt;
		if(effect.j <= 0) return false;
		else return true;
	}
	case effFlicker:
	{
		effect.j -= dt;
		if(effect.j <= 0) return false;
		else return true;
	}
	case effExplosion:
	{
		effect.j -= dt;
		if(effect.j <= 0) return false;
		else return true;
	}
	}
}

int findEffectI(effecType type, int i)
{
	for(int n = 0; n < effects.size(); n++)
		if(effects.at(n).i==i && effects.at(n).type==type) return n;
	return -1;
}

int findEffectJ(effecType type, int j)
{
	for(int n = 0; n < effects.size(); n++)
		if(effects.at(n).j==j && effects.at(n).type==type) return n;
	return -1;
}

int findEffectK(effecType type, int k)
{
	for(int n = 0; n < effects.size(); n++)
		if(effects.at(n).k==k && effects.at(n).type==type) return n;
	return -1;
}

int findEffectX(effecType type, float x)
{
	for(int n = 0; n < effects.size(); n++)
		if(effects.at(n).x==x && effects.at(n).type==type) return n;
	return -1;
}

int findEffectY(effecType type, float y)
{
	for(int n = 0; n < effects.size(); n++)
		if(effects.at(n).y==y && effects.at(n).type==type) return n;
	return -1;
}

int findEffectZ(effecType type, float z)
{
	for(int n = 0; n < effects.size(); n++)
		if(effects.at(n).z==z && effects.at(n).type==type) return n;
	return -1;
}