#ifndef STATE_H
#define STATE_H

#include "config.h"
#include <GLFW/glfw3.h>
#include <vector>

// Game State
extern State gState;
extern AttackType gAttackType;
extern float countdownTimer;

// Objects
extern Missile missile;
extern Drone drone;
extern Bomb bomb;

// Vectors
extern std::vector<Particle> particles;
extern std::vector<Fragment> fragments;
extern std::vector<NPC> npcs;
extern std::vector<Cow> cows;

// Input & Settings
extern bool keys[1024];
extern bool musicMuted;
extern bool playerCowActive;
extern Cow playerCow;

// Camera & Effects
extern float flashAlpha;
extern float shakeTime;
extern float shakeMag;
extern float curShakeX;
extern float curShakeY;

// Textures
extern GLuint texSceneBefore;
extern GLuint texSceneAfterMissile;
extern GLuint texSceneAfterDrone;
extern GLuint texSceneAfterEarthquake;
extern GLuint texMissileSprite;
extern GLuint texDroneSprite;
extern GLuint texBombSprite;
extern GLuint texParticle;
extern GLuint texNPCSprite;

#endif // STATE_H
