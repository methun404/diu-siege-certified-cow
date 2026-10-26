#include "game.h"
#include "state.h"
#include "entities.h"
#include "graphics.h"
#include "audio.h"

void initMissile(){
    missile.x = MSX; missile.y = MSY;
    float dx = MTX - MSX;
    float dy = MTY - MSY;
    float len = sqrtf(dx*dx + dy*dy);
    missile.vx = (dx/len) * 45.0f;
    missile.vy = (dy/len) * 45.0f;
    missile.angle = atan2f(missile.vy, missile.vx);
    missile.active = true;
}

void initDrone(){
    drone.x = WL - 10.0f;
    drone.y = WT - 15.0f;
    drone.vx = 55.0f;
    drone.propAngle = 0;
    drone.active = true;
    drone.bombDropped = false;
    bomb.active = false;
}

void triggerShake(float mag, float time){
    shakeMag = mag;
    shakeTime = time;
}

void update(float dt){
    updateNPCs(dt);
    updateCows(dt);
    
    if (playerCowActive && playerCow.state != CowState::DEAD) {
        if (gState == State::COUNTDOWN || gState == State::EXPLODING) {
            playerCow.state = CowState::PANIC;
            playerCow.dir = (playerCow.x < BX) ? -1 : 1;
            playerCow.vx = playerCow.dir * playerCow.speed * 4.0f;
            playerCow.vy = 0.0f;
            playerCow.angle = 0.0f;
        } else {
            if (keys[GLFW_KEY_G]) {
                playerCow.state = CowState::EAT;
                playerCow.vx = 0;
                playerCow.vy = 0;
            } else {
                playerCow.state = CowState::WANDER;
                playerCow.vx = 0;
                playerCow.vy = 0;
                if (keys[GLFW_KEY_LEFT]) { playerCow.vx = -playerCow.speed; playerCow.dir = -1; }
                if (keys[GLFW_KEY_RIGHT]) { playerCow.vx = playerCow.speed; playerCow.dir = 1; }
                if (keys[GLFW_KEY_UP]) { playerCow.vy = playerCow.speed; }
                if (keys[GLFW_KEY_DOWN]) { playerCow.vy = -playerCow.speed; }
                
                if (playerCow.vx != 0 || playerCow.vy != 0) {
                    playerCow.angle = atan2f(playerCow.vy, fabsf(playerCow.vx)) * 180.0f / 3.14159f * 0.25f; 
                }
            }
        }
        playerCow.x += playerCow.vx * dt;
        playerCow.y += playerCow.vy * dt;
        
        if (playerCow.y > -25.0f) playerCow.y = -25.0f; 
        if (playerCow.y < -75.0f) playerCow.y = -75.0f; 
        
        if (playerCow.x < WL - 10.0f || playerCow.x > WR + 10.0f) playerCow.state = CowState::DEAD;
        if (gState == State::EXPLODING && playerCow.x > B_L - 10.0f && playerCow.x < B_R + 10.0f) playerCow.state = CowState::DEAD;
    }
    
    if(shakeTime>0){
        shakeTime -= dt;
        float damping = expf(-3.0f * (1.8f - shakeTime));
        curShakeX = sinf(shakeTime * 45.0f) * shakeMag * damping;
        curShakeY = cosf(shakeTime * 50.0f) * shakeMag * damping;
        if(shakeTime<=0) { curShakeX = curShakeY = 0; }
    }
    if(flashAlpha>0) flashAlpha -= dt*2.0f;

    if (gState == State::COUNTDOWN) {
        countdownTimer -= dt;
        if(gAttackType == AttackType::EARTHQUAKE) {
            triggerShake(1.5f, 0.1f); 
        }
        if (countdownTimer <= 0.0f) {
            if (gAttackType == AttackType::MISSILE) {
                initMissile();
                gState = State::FLYING;
                playSound(sndMissileLaunch, 1.0f);
            } else if (gAttackType == AttackType::DRONE) {
                initDrone();
                gState = State::FLYING;
                playSound(sndDroneHum, 0.85f);
            } else if (gAttackType == AttackType::EARTHQUAKE) {
                spawnExplosion(BX, GROUND_Y, AttackType::EARTHQUAKE);
                initFragments(AttackType::EARTHQUAKE);
                triggerShake(6.0f, 4.5f);
                gState = State::EXPLODING;
                playSound(sndEarthquake, 1.0f);
            }
        }
    }

    static float smokeT=0;

    if(gState==State::FLYING && gAttackType == AttackType::MISSILE && missile.active){
        float dx = MTX - missile.x;
        float dy = MTY - missile.y;
        float targetAngle = atan2f(dy,dx);
        
        float diff = targetAngle - missile.angle;
        while(diff >  3.14159f) diff -= 2*3.14159f;
        while(diff < -3.14159f) diff += 2*3.14159f;
        
        missile.angle += diff * 15.0f * dt; 
        missile.vx += cosf(missile.angle) * MISSILE_THRUST * dt;
        missile.vy += sinf(missile.angle) * MISSILE_THRUST * dt;
        missile.vy -= GRAVITY * 0.3f * dt; 
        missile.vx *= AIR_DRAG; missile.vy *= AIR_DRAG;
        
        missile.angle = atan2f(missile.vy, missile.vx);
        missile.x += missile.vx * dt; missile.y += missile.vy * dt;

        smokeT += dt;
        if(smokeT>0.01f){
            spawnTrail(missile.x - cosf(missile.angle)*8.0f, missile.y - sinf(missile.angle)*8.0f);
            smokeT=0;
        }

        if(missile.x>=B_L && missile.x<=B_R && missile.y>=B_B && missile.y<=B_T){
            spawnExplosion(missile.x, missile.y, AttackType::MISSILE);
            initFragments(AttackType::MISSILE);
            flashAlpha = 1.0f;
            triggerShake(3.5f, 1.8f);
            missile.active = false;
            gState = State::EXPLODING;
            playSound(sndExplosion, 1.0f);
        }
    }

    if (gAttackType == AttackType::DRONE) {
        if (drone.active) {
            drone.x += drone.vx * dt;
            drone.y += sinf(glfwGetTime() * 3.0f) * 2.5f * dt; 
            drone.propAngle += 1500.0f * dt;
            
            if (gState == State::FLYING && !drone.bombDropped && drone.x >= MTX - 15.0f) {
                drone.bombDropped = true;
                bomb.x = drone.x;
                bomb.y = drone.y - 4.0f;
                bomb.vx = drone.vx * 0.4f; 
                bomb.vy = -2.0f; 
                bomb.angle = 0.0f;
                playSound(sndBombDrop, 0.9f);
                bomb.active = true;
            }
            if (drone.x > WR + 25.0f) {
                drone.active = false;
                stopSound(sndDroneHum);
            }
        }

        if (gState == State::FLYING && bomb.active) {
            bomb.x += bomb.vx * dt;
            bomb.vy -= GRAVITY * dt;
            bomb.y += bomb.vy * dt;
            bomb.angle -= 5.0f * dt; 
            
            smokeT += dt;
            if(smokeT > 0.02f){
                spawnTrail(bomb.x, bomb.y + 4.0f);
                smokeT = 0;
            }
            
            if(bomb.y <= 5.0f){
                spawnExplosion(bomb.x, bomb.y, AttackType::DRONE);
                initFragments(AttackType::DRONE);
                flashAlpha = 1.0f;
                triggerShake(3.5f, 1.8f);
                bomb.active = false;
                gState = State::EXPLODING;
                stopSound(sndDroneHum);
                playSound(sndExplosion, 1.0f);
            }
        }
    }

    if(gState==State::EXPLODING && flashAlpha<=0 && fragments.front().life<=0)
        gState=State::DESTROYED;

    static float treeFireT = 0;
    if((gState==State::EXPLODING || gState==State::DESTROYED) && gAttackType != AttackType::EARTHQUAKE){
        treeFireT += dt;
        if(treeFireT > 0.08f){
            auto spawnFire = [](float tx, float ty) {
                Particle p;
                p.x = tx + ((rand()%40)-20)*0.2f; p.y = ty + ((rand()%40)-20)*0.2f;
                p.vx = ((rand()%20)-10)*0.1f; p.vy = 2.0f + (rand()%10)*0.5f;
                p.life = p.maxLife = 1.0f + (rand()%50)*0.01f;
                p.size = 2.5f + (rand()%20)*0.1f;
                p.r = 1.0f; p.g = 0.5f + (rand()%30)*0.01f; p.b = 0.0f; p.a = 0.85f; p.type = 1;
                particles.push_back(p);
                p.r = p.g = p.b = 0.2f; p.a = 0.6f; p.type = 0;
                p.vy += 1.5f; p.life += 1.0f;
                particles.push_back(p);
            };
            spawnFire(-40.0f, -5.0f); 
            spawnFire(35.0f, -6.0f);  
            treeFireT = 0;
        }
    }

    for(auto& p:particles){
        p.life-=dt;
        p.x+=p.vx*dt; p.y+=p.vy*dt;
        if(p.type==0 || p.type==4){ 
            p.vx *= 0.98f; p.vy *= 0.98f; p.size += dt*1.2f;
        }else if(p.type==1){ 
            p.vy += dt*5.0f; p.vx *= 0.95f; p.size += dt*0.5f;
            if (p.life/p.maxLife > 0.5f) p.g -= 0.01f; 
        }else if(p.type==2){ 
            p.vy -= GRAVITY * dt; p.vx *= 0.99f;
            if(p.y < GROUND_Y){ p.y=GROUND_Y; p.vy *= -0.4f; p.vx *= 0.6f; }
        }
    }
    particles.erase(std::remove_if(particles.begin(),particles.end(),
        [](const Particle& p){return p.life<=0;}),particles.end());

    for(auto& f:fragments){
        if(f.life<=0) continue;
        f.life -= dt*0.12f; 
        f.vy -= GRAVITY * dt;
        f.vx *= 0.99f; f.vy *= 0.99f;
        f.x += f.vx * dt; f.y += f.vy * dt;
        f.angle += f.rotSpeed * dt;
        
        if(f.y - f.h*0.5f < GROUND_Y){
            f.y = GROUND_Y + f.h*0.5f;
            f.vy *= -0.3f; f.vx *= 0.7f; f.rotSpeed *= 0.5f;
        }
    }
}

void render(float dt){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(curShakeX, curShakeY, 0);

    if (gState == State::IDLE || gState == State::COUNTDOWN || gState == State::FLYING) {
        drawScene(texSceneBefore);
    } else {
        if(gAttackType == AttackType::MISSILE) drawScene(texSceneAfterMissile);
        else if(gAttackType == AttackType::DRONE) drawScene(texSceneAfterDrone);
        else if(gAttackType == AttackType::EARTHQUAKE) drawScene(texSceneAfterEarthquake);
    }

    if(gState==State::FLYING){
        if(gAttackType == AttackType::MISSILE && missile.active){
            drawSprite(texMissileSprite, missile.x, missile.y, 20.0f, 8.0f, missile.angle, false);
        } else if(gAttackType == AttackType::DRONE){
            if(bomb.active) {
                drawSprite(texBombSprite, bomb.x, bomb.y, 8.0f, 20.0f, bomb.angle, false);
            }
        }
    }
    
    if (gAttackType == AttackType::DRONE && drone.active) {
        float tilt = sinf(glfwGetTime() * 8.0f) * 0.08f; 
        drawSprite(texDroneSprite, drone.x, drone.y, 25.0f, 25.0f, tilt, false);
    }

    drawNPCs();
    drawCows();
    drawParticles();
    if(!fragments.empty()) drawFragments();

    if(flashAlpha>0){
        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0f,0.9f,0.7f, std::min(flashAlpha*0.7f, 1.0f));
        fillRect(WL,WB,WR,WT);
    }
    
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(gState == State::COUNTDOWN) {
        drawCountdownUI(dt);
    }
    
    glColor4f(0.05f, 0.05f, 0.05f, 0.88f);
    fillRect(WL, WT - 12.0f, WR, WT);
    
    float accentR=0.0f,accentG=0.85f,accentB=0.2f;
    if(gState==State::COUNTDOWN){accentR=1.0f;accentG=0.5f;accentB=0.0f;}
    else if(gState==State::FLYING){accentR=1.0f;accentG=0.1f;accentB=0.0f;}
    else if(gState==State::EXPLODING||gState==State::DESTROYED){accentR=1.0f;accentG=0.0f;accentB=0.0f;}
    glColor4f(accentR,accentG,accentB,1.0f);
    fillRect(WL, WT-12.0f, WR, WT-11.6f);
    
    glColor4f(accentR*0.85f+0.1f, accentG*0.85f+0.1f, 0.35f, 1.0f);
    drawString(WL+2.5f, WT-7.5f, ">> TACTICAL STRIKE SIM", GLUT_BITMAP_HELVETICA_18);
    
    const char* stateStr = "--  STANDBY  --";
    float sr=0.4f,sg=1.0f,sb=0.5f;
    if(gState==State::COUNTDOWN){stateStr="[!] LAUNCH SEQUENCE";sr=1.0f;sg=0.75f;sb=0.0f;}
    else if(gState==State::FLYING){stateStr="[>>] STRIKE INBOUND";sr=1.0f;sg=0.25f;sb=0.25f;}
    else if(gState==State::EXPLODING){stateStr="[**] IMPACT";sr=1.0f;sg=0.1f;sb=0.0f;}
    else if(gState==State::DESTROYED){stateStr="[X] TARGET DESTROYED  |  SPACE = RESET";sr=1.0f;sg=0.35f;sb=0.35f;}
    
    glColor4f(0.0f,0.0f,0.0f,0.55f);
    drawString(-18.4f, WT-7.7f, stateStr, GLUT_BITMAP_HELVETICA_18);
    glColor4f(sr,sg,sb,1.0f);
    drawString(-18.0f, WT-7.5f, stateStr, GLUT_BITMAP_HELVETICA_18);
    
    const char* modeStr="MODE: STANDBY";
    if(gAttackType==AttackType::MISSILE) modeStr="MODE: CRUISE MISSILE";
    else if(gAttackType==AttackType::DRONE) modeStr="MODE: DRONE BOMBER";
    else if(gAttackType==AttackType::EARTHQUAKE) modeStr="MODE: SEISMIC CHARGE";
    glColor4f(0.0f,0.0f,0.0f,0.55f);
    drawStringSmall(28.4f, WT-7.7f, modeStr);
    glColor4f(0.7f,0.7f,0.7f,1.0f);
    drawStringSmall(28.0f, WT-7.5f, modeStr);
    
    if (musicMuted) {
        glColor4f(1.0f, 0.3f, 0.3f, 0.9f);
        drawStringSmall(WR-32.0f, WT-5.5f, "[MUTED]");
    } else {
        glColor4f(0.0f, 0.85f, 0.3f, 0.9f);
        drawStringSmall(WR-32.0f, WT-5.5f, "[MUSIC ON]");
    }
    glColor4f(0.5f,0.5f,0.5f,0.75f);
    drawStringSmall(WR-32.0f, WT-10.0f, "S=Mute  H=Controls");
    
    bool showHelp = keys[GLFW_KEY_H];
    
    if (showHelp) {
        glColor4f(0.03f, 0.05f, 0.03f, 0.92f);
        fillRect(WL, WB, WR, WB + 14.0f);
        glColor4f(accentR,accentG,accentB,1.0f);
        fillRect(WL, WB+13.7f, WR, WB+14.0f);
        
        auto drawKeyBtn = [&](float bx, float by, const char* key, const char* label, bool active, float r, float g, float b) {
            glColor4f(active ? r : 0.12f, active ? g : 0.12f, active ? b : 0.12f, 0.95f);
            fillRect(bx, by, bx+12.0f, by+10.0f);
            glColor4f(active ? r : 0.0f, active ? g : accentG*0.6f, active ? b : accentB*0.6f, 1.0f);
            glLineWidth(1.8f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(bx,by);glVertex2f(bx+12.0f,by);glVertex2f(bx+12.0f,by+10.0f);glVertex2f(bx,by+10.0f);
            glEnd();
            glLineWidth(1.0f);
            glColor4f(0.0f,0.0f,0.0f,0.7f);
            drawString(bx+4.4f, by+7.0f, key, GLUT_BITMAP_HELVETICA_18);
            glColor4f(1.0f, 0.95f, 0.0f, 1.0f);
            drawString(bx+4.0f, by+7.3f, key, GLUT_BITMAP_HELVETICA_18);
            glColor4f(0.0f,0.0f,0.0f,0.6f);
            drawStringSmall(bx+0.8f, by+1.9f, label);
            glColor4f(0.85f,0.85f,0.85f,1.0f);
            drawStringSmall(bx+0.5f, by+2.2f, label);
        };
        
        bool isMissile=gAttackType==AttackType::MISSILE;
        bool isDrone=gAttackType==AttackType::DRONE;
        bool isEQ=gAttackType==AttackType::EARTHQUAKE;
        
        float by = WB+2.0f;
        drawKeyBtn(WL+2.0f,  by, "M", "MISSILE",     isMissile,  0.8f,0.2f,0.0f);
        drawKeyBtn(WL+16.0f, by, "D", "DRONE",       isDrone,    0.0f,0.4f,0.9f);
        drawKeyBtn(WL+30.0f, by, "E", "EARTHQUAKE",  isEQ,       0.6f,0.3f,0.0f);
        drawKeyBtn(WL+44.0f, by, "C", "SPAWN COW",   playerCowActive, 0.0f,0.5f,0.2f);
        drawKeyBtn(WL+58.0f, by, "G", "EAT GRASS",   keys[GLFW_KEY_G], 0.0f,0.4f,0.1f);
        drawKeyBtn(WL+72.0f, by, "SPC","RESET",      false,      0.4f,0.4f,0.0f);
        
        glColor4f(0.12f,0.04f,0.04f,0.95f);
        fillRect(WR-16.0f, by, WR-2.0f, by+10.0f);
        glColor4f(0.9f,0.1f,0.1f,1.0f);
        glLineWidth(1.8f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(WR-16.0f,by);glVertex2f(WR-2.0f,by);glVertex2f(WR-2.0f,by+10.0f);glVertex2f(WR-16.0f,by+10.0f);
        glEnd();
        glLineWidth(1.0f);
        glColor4f(0.0f,0.0f,0.0f,0.7f);
        drawString(WR-13.0f, by+7.2f, "ESC", GLUT_BITMAP_HELVETICA_18);
        glColor4f(1.0f,0.3f,0.3f,1.0f);
        drawString(WR-13.4f, by+7.5f, "ESC", GLUT_BITMAP_HELVETICA_18);
        glColor4f(0.0f,0.0f,0.0f,0.6f);
        drawStringSmall(WR-15.0f, by+2.0f, "QUIT GAME");
        glColor4f(0.85f,0.85f,0.85f,1.0f);
        drawStringSmall(WR-15.3f, by+2.3f, "QUIT GAME");
    }
    
    glLoadIdentity();
}

void resetScene(){
    particles.clear();
    fragments.clear();
    flashAlpha = shakeTime = 0; 
    curShakeX = curShakeY = 0;
    gState = State::IDLE;
    gAttackType = AttackType::NONE;
    playerCowActive = false;
    initNPCs();
    
    stopSound(sndMissileLaunch);
    stopSound(sndExplosion);
    stopSound(sndDroneHum);
    stopSound(sndEarthquake);
    stopSound(sndAlarm);
    stopSound(sndBombDrop);
}

void triggerAttack(AttackType type) {
    if(gState!=State::IDLE)return;
    gState = State::COUNTDOWN;
    gAttackType = type;
    countdownTimer = 3.99f;
    playSound(sndAlarm, 0.9f);
}
