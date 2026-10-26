#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

// ═══════════════════════════════════════════════════
//  WORLD COORDINATE SYSTEM
// ═══════════════════════════════════════════════════
const int   WIN_W = 1280;
const int   WIN_H =  720;
const float WL = -100.0f;
const float WR =  100.0f;
const float WB =  -56.25f;
const float WT =   56.25f;

// ── Building destruction bounds ───────
const float BX  =  -5.0f;
const float BY  =  15.0f;
const float BW  =  75.0f;
const float BH  =  65.0f;
const float B_L = BX - BW * 0.5f;
const float B_R = BX + BW * 0.5f;
const float B_B = BY - BH * 0.5f;
const float B_T = BY + BH * 0.5f;

const float GROUND_Y = -35.0f; 

// ── Missile Target ────────────────────────────────────────────
const float MSX = -115.0f, MSY = 55.0f;
const float MTX = BX,      MTY = BY - 10.0f; 

// ── Physics Constants ─────────────────────────────────────────
const float GRAVITY = 35.0f;
const float AIR_DRAG = 0.98f;
const float MISSILE_THRUST = 85.0f;
const float BOMB_SPEED = 20.0f;

// ─── State ─────────────────────────────────────────────────────
enum class State { IDLE, COUNTDOWN, FLYING, EXPLODING, DESTROYED };
enum class AttackType { NONE, MISSILE, DRONE, EARTHQUAKE };

// ─── Structs ───────────────────────────────────────────────────
struct Missile { float x,y,vx,vy,angle; bool active; };
struct Drone { float x, y, vx; float propAngle; bool active, bombDropped; };
struct Bomb { float x, y, vx, vy; float angle; bool active; };
struct Particle { float x,y,vx,vy,life,maxLife,size,r,g,b,a; int type; };
struct Fragment { float x,y,vx,vy,angle,rotSpeed,w,h,life,tx,ty,tw,th; };

enum class NPCState { WANDER, PANIC, DEAD };
struct NPC { float x,y,vx,speed; NPCState state; float timer; int dir; float animOffset; };

enum class CowState { WANDER, EAT, PANIC, DEAD };
struct Cow { float x,y,vx,vy,speed; CowState state; float timer; int dir; float animOffset; float angle; };

#endif // CONFIG_H
