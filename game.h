#ifndef GAME_H
#define GAME_H

#include "config.h"

void initMissile();
void initDrone();
void triggerShake(float mag, float time);

void update(float dt);
void render(float dt);

void resetScene();
void triggerAttack(AttackType type);

#endif // GAME_H
