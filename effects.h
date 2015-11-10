#ifndef EFFECTS_H
#define EFFECTS_H


enum effecType{effShine, effFlicker, effExplosion};

typedef struct Effect
{
	effecType type;
	float x, y, z;
	int i, j, k;
} Effect;

void addEffect(effecType type, float x, float y, float z, int i, int j, int k);
void addShineEffect(int i);
void addFlickerEffect(int i);
void addExplosionEffect(float x, float y, float z);

bool moveOnEffect(Effect & effect, int dt);

int findEffectI(effecType type, int i);
int findEffectJ(effecType type, int j);
int findEffectK(effecType type, int k);
int findEffectX(effecType type, float x);
int findEffectY(effecType type, float y);
int findEffectZ(effecType type, float z);

#endif