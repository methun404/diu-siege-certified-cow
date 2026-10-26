#include "entities.h"
#include "state.h"
#include "graphics.h"
#include <GLFW/glfw3.h>

void spawnTrail(float x, float y)
{
    Particle p;
    p.x = x + ((rand() % 20) - 10) * 0.1f;
    p.y = y + ((rand() % 20) - 10) * 0.1f;
    p.vx = ((rand() % 50) - 25) * 0.01f;
    p.vy = ((rand() % 50) - 25) * 0.01f;
    p.life = p.maxLife = 0.8f + (rand() % 30) * 0.01f;
    p.size = 1.5f + (rand() % 25) * 0.1f;
    float sh = 0.7f + (rand() % 20) * 0.01f;
    p.r = sh;
    p.g = sh;
    p.b = sh;
    p.a = 0.5f;
    p.type = 0;
    particles.push_back(p);
}

void spawnExplosion(float cx, float cy, AttackType type)
{
    // Shockwave
    Particle sw;
    sw.x = cx;
    sw.y = cy;
    sw.vx = sw.vy = 0;
    sw.life = sw.maxLife = (type == AttackType::EARTHQUAKE) ? 0.8f : 0.4f;
    sw.size = (type == AttackType::EARTHQUAKE) ? 4.0f : 2.0f;
    sw.r = 1;
    sw.g = 1;
    sw.b = 1;
    sw.a = 0.8f;
    sw.type = 3;
    particles.push_back(sw);

    int fireCount = (type == AttackType::EARTHQUAKE) ? 20 : 150;
    if (type == AttackType::DRONE)
        fireCount = 200;

    // Fireball
    for (int i = 0; i < fireCount; i++)
    {
        Particle p;
        float a = (float)(rand() % 360) * 3.14159f / 180.0f;
        float spd = 0.5f + (rand() % 100) * 0.25f;
        p.x = cx + cosf(a) * 1.0f;
        p.y = cy + sinf(a) * 1.0f;
        p.vx = cosf(a) * spd;
        p.vy = sinf(a) * spd + (rand() % 20) * 0.2f;
        p.life = p.maxLife = 0.5f + (rand() % 50) * 0.01f;
        p.size = 1.0f + (rand() % 40) * 0.1f;
        p.r = 1.0f;
        p.g = 0.3f + (rand() % 50) * 0.01f;
        p.b = 0.0f;
        p.a = 0.9f;
        p.type = 1;
        particles.push_back(p);
    }

    // Heavy Smoke/Dust
    int smokeCount = (type == AttackType::EARTHQUAKE) ? 250 : 100;
    for (int i = 0; i < smokeCount; i++)
    {
        Particle p;
        float a = (float)(rand() % 360) * 3.14159f / 180.0f;
        float spd = 0.2f + (rand() % 100) * 0.1f;
        p.x = cx + ((rand() % 100) - 50) * 0.5f;
        p.y = cy + ((rand() % 100) - 50) * 0.2f;
        p.vx = cosf(a) * spd;
        p.vy = sinf(a) * spd + ((type == AttackType::EARTHQUAKE) ? 1.0f : 3.0f);
        p.life = p.maxLife = 3.0f + (rand() % 100) * 0.02f;
        p.size = 1.5f + (rand() % 30) * 0.1f;
        if (type == AttackType::EARTHQUAKE)
        {
            p.r = 0.6f;
            p.g = 0.5f;
            p.b = 0.4f;
            p.a = 0.9f;
            p.type = 4;
        }
        else
        {
            p.r = p.g = p.b = 0.2f + (rand() % 20) * 0.01f;
            p.a = 0.8f;
            p.type = 0;
        }
        particles.push_back(p);
    }

    if (type != AttackType::EARTHQUAKE)
    {
        for (int i = 0; i < 200; i++)
        {
            Particle p;
            float a = (float)(rand() % 360) * 3.14159f / 180.0f;
            float spd = 10.0f + (rand() % 100) * 0.3f;
            p.x = cx;
            p.y = cy;
            p.vx = cosf(a) * spd;
            p.vy = sinf(a) * spd;
            p.life = p.maxLife = 1.5f + (rand() % 50) * 0.02f;
            p.size = 0.15f + (rand() % 10) * 0.05f;
            p.r = 1.0f;
            p.g = 0.8f;
            p.b = 0.2f;
            p.a = 1.0f;
            p.type = 2;
            particles.push_back(p);
        }
    }
}

void drawParticles()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_TEXTURE_2D);
    for (const auto &p : particles)
    {
        if (p.type == 3)
        {
            float ratio = p.life / p.maxLife;
            glLineWidth(8.0f * ratio);
            glColor4f(p.r, p.g, p.b, p.a * ratio);
            glBegin(GL_LINE_LOOP);
            float currentSize = p.size + (1.0f - ratio) * 60.0f;
            for (int i = 0; i <= 40; i++)
            {
                float a = 2 * 3.14159f * i / 40;
                glVertex2f(p.x + cosf(a) * currentSize, p.y + sinf(a) * currentSize * 0.7f);
            }
            glEnd();
        }
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texParticle);
    for (const auto &p : particles)
    {
        if (p.type != 3)
        {
            float ratio = p.life / p.maxLife;
            float alpha = p.a * ratio;
            float sz = (p.type == 0 || p.type == 4) ? p.size * (2.5f - ratio) : p.size * ratio;
            glColor4f(p.r, p.g, p.b, alpha);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(p.x - sz, p.y - sz);
            glTexCoord2f(1, 0);
            glVertex2f(p.x + sz, p.y - sz);
            glTexCoord2f(1, 1);
            glVertex2f(p.x + sz, p.y + sz);
            glTexCoord2f(0, 1);
            glVertex2f(p.x - sz, p.y + sz);
            glEnd();
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void initFragments(AttackType type)
{
    fragments.clear();
    int rows = 10, cols = 10;
    float fw = BW / cols, fh = BH / rows;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            Fragment f;
            f.w = fw * (0.9f + (rand() % 20) * 0.01f);
            f.h = fh * (0.9f + (rand() % 20) * 0.01f);
            f.x = B_L + c * fw + fw * 0.5f;
            f.y = B_B + r * fh + fh * 0.5f;

            f.tx = (f.x - f.w * 0.5f + 100.0f) / 200.0f;
            f.ty = (f.y - f.h * 0.5f + 100.0f) / 200.0f;
            f.tw = f.w / 200.0f;
            f.th = f.h / 200.0f;

            f.angle = (float)(rand() % 360);
            f.rotSpeed = ((rand() % 100) - 50) * 1.5f;
            f.life = 1.0f;

            if (type == AttackType::EARTHQUAKE)
            {
                f.vx = ((rand() % 100) - 50) * 0.1f;
                f.vy = -5.0f - ((rand() % 100) * 0.1f);
                f.rotSpeed *= 0.5f;
            }
            else
            {
                float dx = f.x - MTX, dy = f.y - MTY;
                float dist = sqrtf(dx * dx + dy * dy) + 1.0f;
                float force = 1500.0f / (dist * dist);
                f.vx = (dx / dist) * force + ((rand() % 100) - 50) * 0.2f;
                f.vy = (dy / dist) * force + ((rand() % 100) - 50) * 0.2f + 15.0f;
            }
            fragments.push_back(f);
        }
    }
}

void drawFragments()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texSceneBefore);
    for (const auto &f : fragments)
    {
        glPushMatrix();
        glTranslatef(f.x, f.y, 0);
        glRotatef(f.angle, 0, 0, 1);
        glColor4f(1, 1, 1, f.life);
        float hw = f.w * 0.5f, hh = f.h * 0.5f;
        glBegin(GL_QUADS);
        glTexCoord2f(f.tx, f.ty);
        glVertex2f(-hw, -hh);
        glTexCoord2f(f.tx + f.tw, f.ty);
        glVertex2f(hw, -hh);
        glTexCoord2f(f.tx + f.tw, f.ty + f.th);
        glVertex2f(hw, hh);
        glTexCoord2f(f.tx, f.ty + f.th);
        glVertex2f(-hw, hh);
        glEnd();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
}

void initNPCs()
{
    npcs.clear();
    for (int i = 0; i < 35; i++)
    {
        NPC n;
        n.x = WL + 10.0f + (rand() % 180);
        if (n.x > B_L - 10.0f && n.x < B_R + 10.0f)
            n.x += (rand() % 2 == 0 ? 40.0f : -40.0f);

        if (i < 15)
            n.y = -65.0f - (rand() % 10);
        else
            n.y = -20.0f - (rand() % 25);

        n.speed = 3.0f + (rand() % 20) * 0.1f;
        n.state = NPCState::WANDER;
        n.timer = (rand() % 30) * 0.1f;
        n.dir = (rand() % 2 == 0) ? 1 : -1;
        n.vx = n.dir * n.speed;
        n.animOffset = (rand() % 100) * 0.1f;
        npcs.push_back(n);
    }

    cows.clear();
    for (int i = 0; i < 8; i++)
    {
        Cow c;
        c.x = WL + 20.0f + (rand() % 160);
        if (c.x > B_L - 15.0f && c.x < B_R + 15.0f)
            c.x += (rand() % 2 == 0 ? 40.0f : -40.0f);
        c.y = -20.0f - (rand() % 20);
        c.speed = 1.5f + (rand() % 10) * 0.1f;
        c.state = CowState::EAT;
        c.timer = 2.0f + (rand() % 30) * 0.1f;
        c.dir = (rand() % 2 == 0) ? 1 : -1;
        c.vx = 0;
        c.vy = 0;
        c.animOffset = (rand() % 100) * 0.1f;
        c.angle = 0.0f;
        cows.push_back(c);
    }
}

void updateNPCs(float dt)
{
    for (auto &n : npcs)
    {
        if (n.state == NPCState::DEAD)
            continue;

        if (gState == State::COUNTDOWN || gState == State::EXPLODING)
        {
            n.state = NPCState::PANIC;
        }

        if (n.state == NPCState::WANDER)
        {
            n.timer -= dt;
            if (n.timer <= 0)
            {
                n.timer = 1.0f + (rand() % 30) * 0.1f;
                int action = rand() % 3;
                if (action == 0)
                    n.vx = 0;
                else
                {
                    n.dir = (rand() % 2 == 0) ? 1 : -1;
                    n.vx = n.dir * n.speed * 0.5f;
                }
            }
            if (n.x < WL + 5.0f)
            {
                n.dir = 1;
                n.vx = n.speed * 0.5f;
            }
            if (n.x > WR - 5.0f)
            {
                n.dir = -1;
                n.vx = -n.speed * 0.5f;
            }
        }
        else if (n.state == NPCState::PANIC)
        {
            n.dir = (n.x < BX) ? -1 : 1;
            n.vx = n.dir * n.speed * 3.5f;
        }

        n.x += n.vx * dt;

        if (n.x < WL - 5.0f || n.x > WR + 5.0f)
        {
            n.state = NPCState::DEAD;
        }

        if (gState == State::EXPLODING && n.x > B_L - 10.0f && n.x < B_R + 10.0f)
        {
            n.state = NPCState::DEAD;
        }
    }
}

void drawProceduralNPC(float x, float y, float speed, float time, bool flip, bool isPanic)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    if (flip)
        glScalef(-1, 1, 1);

    float animSpeed = speed * (isPanic ? 3.0f : 1.5f);
    if (speed < 0.1f)
        animSpeed = 0.0f;
    float t = time * animSpeed;

    float upperLeg = 1.0f;
    float lowerLeg = 1.0f;
    float torsoLen = 1.5f;

    float bounce = (animSpeed > 0) ? fabsf(sinf(t)) * 0.3f : 0.0f;
    glTranslatef(0, upperLeg + lowerLeg + bounce, 0);
    float lean = isPanic ? -25.0f : 0.0f;

    glDisable(GL_TEXTURE_2D);
    glColor4f(0.05f, 0.05f, 0.05f, 1.0f);
    glLineWidth(4.0f);

    // --- Right Arm (Back) ---
    glPushMatrix();
    glRotatef(lean, 0, 0, 1);
    glTranslatef(0, torsoLen * 0.8f, 0);
    float rShoulder = sinf(t) * 45.0f;
    glRotatef(rShoulder, 0, 0, 1);
    drawLineDDA(0, 0, 0, -0.8f);
    glTranslatef(0, -0.8f, 0);
    glRotatef(isPanic ? 70.0f : 20.0f, 0, 0, 1);
    drawLineDDA(0, 0, 0, -0.7f);
    glPopMatrix();

    // --- Right Leg (Back) ---
    glPushMatrix();
    float rThigh = sinf(t + 3.14159f) * 45.0f;
    glRotatef(rThigh, 0, 0, 1);
    drawLineDDA(0, 0, 0, -upperLeg);
    glTranslatef(0, -upperLeg, 0);
    float rKnee = std::min(0.0f, sinf(t + 3.14159f) * 60.0f);
    glRotatef(rKnee, 0, 0, 1);
    drawLineDDA(0, 0, 0, -lowerLeg);
    glPopMatrix();

    // --- Torso ---
    glPushMatrix();
    glRotatef(lean, 0, 0, 1);
    drawLineDDA(0, 0, 0, torsoLen);

    // --- Head ---
    glTranslatef(0, torsoLen + 0.4f, 0);
    drawCircleMidpoint(0, 0, 0.4f, true);
    glPopMatrix();

    // --- Left Leg (Front) ---
    glPushMatrix();
    float lThigh = sinf(t) * 45.0f;
    glRotatef(lThigh, 0, 0, 1);
    drawLineDDA(0, 0, 0, -upperLeg);
    glTranslatef(0, -upperLeg, 0);
    float lKnee = std::min(0.0f, sinf(t) * 60.0f);
    glRotatef(lKnee, 0, 0, 1);
    drawLineDDA(0, 0, 0, -lowerLeg);
    glPopMatrix();

    // --- Left Arm (Front) ---
    glPushMatrix();
    glRotatef(lean, 0, 0, 1);
    glTranslatef(0, torsoLen * 0.8f, 0);
    float lShoulder = sinf(t + 3.14159f) * 45.0f;
    glRotatef(lShoulder, 0, 0, 1);
    drawLineDDA(0, 0, 0, -0.8f);
    glTranslatef(0, -0.8f, 0);
    glRotatef(isPanic ? 70.0f : 20.0f, 0, 0, 1);
    drawLineDDA(0, 0, 0, -0.7f);
    glPopMatrix();

    glLineWidth(1.0f);
    glPopMatrix();
}

void drawNPCs()
{
    float time = glfwGetTime();
    for (const auto &n : npcs)
    {
        if (n.state == NPCState::DEAD)
            continue;
        bool flip = (n.dir < 0);
        bool isPanic = (n.state == NPCState::PANIC);
        float currentSpeed = fabsf(n.vx);
        drawProceduralNPC(n.x, n.y, currentSpeed, time + n.animOffset, flip, isPanic);
    }
}

void updateCows(float dt)
{
    for (auto &c : cows)
    {
        if (c.state == CowState::DEAD)
            continue;

        if (gState == State::COUNTDOWN || gState == State::EXPLODING)
        {
            c.state = CowState::PANIC;
        }

        if (c.state == CowState::WANDER || c.state == CowState::EAT)
        {
            c.timer -= dt;
            if (c.timer <= 0)
            {
                c.timer = 2.0f + (rand() % 40) * 0.1f;
                int action = rand() % 3;
                if (action < 2)
                {
                    c.state = CowState::EAT;
                    c.vx = 0;
                }
                else
                {
                    c.state = CowState::WANDER;
                    c.dir = (rand() % 2 == 0) ? 1 : -1;
                    c.vx = c.dir * c.speed;
                }
            }
            if (c.x < WL + 10.0f)
            {
                c.dir = 1;
                c.vx = c.speed;
                c.state = CowState::WANDER;
            }
            if (c.x > WR - 10.0f)
            {
                c.dir = -1;
                c.vx = -c.speed;
                c.state = CowState::WANDER;
            }
        }
        else if (c.state == CowState::PANIC)
        {
            c.dir = (c.x < BX) ? -1 : 1;
            c.vx = c.dir * c.speed * 4.0f;
        }

        c.x += c.vx * dt;
        c.y += c.vy * dt;

        if (c.x < WL - 10.0f || c.x > WR + 10.0f)
            c.state = CowState::DEAD;
        if (gState == State::EXPLODING && c.x > B_L - 10.0f && c.x < B_R + 10.0f)
            c.state = CowState::DEAD;
    }
}

void drawProceduralCow(float x, float y, float speed, float time, bool flip, bool isPanic, bool isEating, bool hasIDCard, float angle)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    if (flip)
        glScalef(-1, 1, 1);

    float torsoH = 1.8f;
    float legLen = 1.5f;
    glTranslatef(0, legLen + torsoH / 2.0f, 0);
    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -(legLen + torsoH / 2.0f), 0);

    float animSpeed = speed * (isPanic ? 3.0f : 1.5f);
    if (speed < 0.1f)
        animSpeed = 0.0f;
    float t = time * animSpeed;

    float torsoLen = 3.5f;

    float bounce = (animSpeed > 0 && !isEating) ? fabsf(sinf(t * 2.0f)) * 0.2f : 0.0f;
    glTranslatef(0, legLen + torsoH + bounce, 0);

    glDisable(GL_TEXTURE_2D);
    glColor4f(0.05f, 0.05f, 0.05f, 1.0f);

    float swing1 = sinf(t) * 30.0f;
    float swing2 = sinf(t + 3.14159f) * 30.0f;
    if (isPanic)
    {
        swing1 = sinf(t) * 50.0f;
        swing2 = sinf(t + 3.14159f) * 50.0f;
    }

    auto drawLeg = [&](float xOffset, float angle)
    {
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        glRotatef(angle, 0, 0, 1);
        glBegin(GL_POLYGON);
        glVertex2f(-0.3f, 0);
        glVertex2f(0.3f, 0);
        glVertex2f(0.2f, -legLen);
        glVertex2f(-0.2f, -legLen);
        glEnd();
        glPopMatrix();
    };

    drawLeg(-torsoLen / 2, swing1);
    drawLeg(-torsoLen / 2, swing2);
    drawLeg(torsoLen / 2, swing1);
    drawLeg(torsoLen / 2, swing2);

    float th = 1.0f;
    glBegin(GL_POLYGON);
    glVertex2f(-torsoLen / 2 - 0.5f, -th);
    glVertex2f(torsoLen / 2 + 0.5f, -th);
    glVertex2f(torsoLen / 2 + 0.5f, th);
    glVertex2f(-torsoLen / 2 - 0.5f, th);
    glEnd();

    glLineWidth(2.0f);
    glPushMatrix();
    glTranslatef(-torsoLen / 2 - 0.5f, 0.5f, 0);
    float tailSwing = sinf(time * 5.0f) * 15.0f - 20.0f;
    if (isPanic)
        tailSwing = 30.0f;
    glRotatef(tailSwing, 0, 0, 1);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(-1.5f, -1.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(torsoLen / 2 + 0.5f, 0, 0);
    float neckAngle = isEating ? -60.0f : (isPanic ? 30.0f : 15.0f);
    float headBob = (animSpeed > 0 && !isEating) ? sinf(t) * 5.0f : 0.0f;
    glRotatef(neckAngle + headBob, 0, 0, 1);

    glBegin(GL_POLYGON);
    glVertex2f(0, -0.6f);
    glVertex2f(1.5f, 0.4f);
    glVertex2f(1.5f, 1.6f);
    glVertex2f(0, 0.6f);
    glEnd();

    glTranslatef(1.5f, 1.0f, 0);
    glRotatef(-30.0f, 0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.6f);
    glVertex2f(1.5f, -0.4f);
    glVertex2f(1.5f, 0.4f);
    glVertex2f(-0.5f, 0.6f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, 0.5f);
    glVertex2f(-0.5f, 1.5f);
    glEnd();
    glPopMatrix();

    if (hasIDCard)
    {
        glPushMatrix();
        glTranslatef(torsoLen / 2 + 0.5f, -0.6f, 0);
        glRotatef(isEating ? -30.0f : 0.0f, 0, 0, 1);
        glColor3f(0.2f, 0.6f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0, -0.8f);
        glEnd();
        glTranslatef(0, -0.8f, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        glVertex2f(-0.3f, -0.4f);
        glVertex2f(0.3f, -0.4f);
        glVertex2f(0.3f, 0.4f);
        glVertex2f(-0.3f, 0.4f);
        glEnd();
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_POLYGON);
        glVertex2f(-0.1f, -0.2f);
        glVertex2f(0.1f, -0.2f);
        glVertex2f(0.1f, 0.2f);
        glVertex2f(-0.1f, 0.2f);
        glEnd();
        glPopMatrix();
        glColor4f(0.05f, 0.05f, 0.05f, 1.0f);
    }

    glLineWidth(1.0f);
    glPopMatrix();
}

void drawCows()
{
    float time = glfwGetTime();
    for (const auto &c : cows)
    {
        if (c.state == CowState::DEAD)
            continue;
        bool flip = (c.dir < 0);
        bool isPanic = (c.state == CowState::PANIC);
        bool isEating = (c.state == CowState::EAT);
        float currentSpeed = sqrtf(c.vx * c.vx + c.vy * c.vy);
        drawProceduralCow(c.x, c.y, currentSpeed, time + c.animOffset, flip, isPanic, isEating, false, c.angle);
    }

    if (playerCowActive && playerCow.state != CowState::DEAD)
    {
        bool flip = (playerCow.dir < 0);
        bool isPanic = (playerCow.state == CowState::PANIC);
        bool isEating = (playerCow.state == CowState::EAT);
        float currentSpeed = sqrtf(playerCow.vx * playerCow.vx + playerCow.vy * playerCow.vy);
        drawProceduralCow(playerCow.x, playerCow.y, currentSpeed, time + playerCow.animOffset, flip, isPanic, isEating, true, playerCow.angle);
    }
}
