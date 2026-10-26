#ifndef ENTITIES_H
#define ENTITIES_H

#include "config.h"

void spawnTrail(float x,float y);
void spawnExplosion(float cx,float cy, AttackType type);
void drawParticles();

void initFragments(AttackType type);
void drawFragments();

void initNPCs();
void updateNPCs(float dt);
void drawNPCs();
void drawProceduralNPC(float x, float y, float speed, float time, bool flip, bool isPanic);

void updateCows(float dt);
void drawCows();
void drawProceduralCow(float x, float y, float speed, float time, bool flip, bool isPanic, bool isEating, bool hasIDCard, float angle);

#endif // ENTITIES_H
