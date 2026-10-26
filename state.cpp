#include "state.h"

// Game State
State gState = State::IDLE;
AttackType gAttackType = AttackType::NONE;
float countdownTimer = 0.0f;

// Objects
Missile missile;
Drone drone;
Bomb bomb;

// Vectors
std::vector<Particle> particles;
std::vector<Fragment> fragments;
std::vector<NPC> npcs;
std::vector<Cow> cows;

// Input & Settings
bool keys[1024] = {false};
bool musicMuted = false;
bool playerCowActive = false;
Cow playerCow;

// Camera & Effects
float flashAlpha = 0;
float shakeTime = 0;
float shakeMag = 0;
float curShakeX = 0;
float curShakeY = 0;

// Textures
GLuint texSceneBefore = 0;
GLuint texSceneAfterMissile = 0;
GLuint texSceneAfterDrone = 0;
GLuint texSceneAfterEarthquake = 0;
GLuint texMissileSprite = 0;
GLuint texDroneSprite = 0;
GLuint texBombSprite = 0;
GLuint texParticle = 0;
GLuint texNPCSprite = 0;
