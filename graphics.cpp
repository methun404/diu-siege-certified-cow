#include "graphics.h"
#include "config.h"
#include "state.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void fillRect(float l, float b, float r, float t)
{
    glBegin(GL_QUADS);
    glVertex2f(l, b);
    glVertex2f(r, b);
    glVertex2f(r, t);
    glVertex2f(l, t);
    glEnd();
}

GLuint loadTexture(const char *path, bool flip)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int w, h, ch;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(path, &w, &h, &ch, 4);
    if (data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        std::cout << "[Tex] " << path << " Loaded.\n";
    }
    else
    {
        std::cout << "[Tex] FAILED: " << path << "\n";
    }
    stbi_image_free(data);
    return id;
}

void drawScene(GLuint texID)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    float vB = (WB + 100.0f) / 200.0f;
    float vT = (WT + 100.0f) / 200.0f;
    glTexCoord2f(0.0f, vB);
    glVertex2f(WL, WB);
    glTexCoord2f(1.0f, vB);
    glVertex2f(WR, WB);
    glTexCoord2f(1.0f, vT);
    glVertex2f(WR, WT);
    glTexCoord2f(0.0f, vT);
    glVertex2f(WL, WT);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawSprite(GLuint tex, float x, float y, float w, float h, float angle, bool flipX)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle * 180.0f / 3.14159f, 0, 0, 1);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    float txL = flipX ? 1.0f : 0.0f;
    float txR = flipX ? 0.0f : 1.0f;
    glTexCoord2f(txL, 0.0f);
    glVertex2f(-w * 0.5f, -h * 0.5f);
    glTexCoord2f(txR, 0.0f);
    glVertex2f(w * 0.5f, -h * 0.5f);
    glTexCoord2f(txR, 1.0f);
    glVertex2f(w * 0.5f, h * 0.5f);
    glTexCoord2f(txL, 1.0f);
    glVertex2f(-w * 0.5f, h * 0.5f);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void draw7SegmentDigit(float x, float y, float size, int num)
{
    glLineWidth(8.0f);
    glColor4f(1.0f, 0.1f, 0.1f, 0.9f);
    glBegin(GL_LINES);

    float hw = size * 0.4f;
    float hh = size * 0.5f;

    bool seg[7] = {0};
    if (num == 0)
    {
        seg[0] = seg[1] = seg[2] = seg[4] = seg[5] = seg[6] = 1;
    }
    if (num == 1)
    {
        seg[2] = seg[5] = 1;
    }
    if (num == 2)
    {
        seg[0] = seg[2] = seg[3] = seg[4] = seg[6] = 1;
    }
    if (num == 3)
    {
        seg[0] = seg[2] = seg[3] = seg[5] = seg[6] = 1;
    }

    // Top
    if (seg[0])
    {
        glVertex2f(x - hw, y + hh);
        glVertex2f(x + hw, y + hh);
    }
    // Top Left
    if (seg[1])
    {
        glVertex2f(x - hw, y + hh);
        glVertex2f(x - hw, y);
    }
    // Top Right
    if (seg[2])
    {
        glVertex2f(x + hw, y + hh);
        glVertex2f(x + hw, y);
    }
    // Middle
    if (seg[3])
    {
        glVertex2f(x - hw, y);
        glVertex2f(x + hw, y);
    }
    // Bottom Left
    if (seg[4])
    {
        glVertex2f(x - hw, y);
        glVertex2f(x - hw, y - hh);
    }
    // Bottom Right
    if (seg[5])
    {
        glVertex2f(x + hw, y);
        glVertex2f(x + hw, y - hh);
    }
    // Bottom
    if (seg[6])
    {
        glVertex2f(x - hw, y - hh);
        glVertex2f(x + hw, y - hh);
    }

    glEnd();
    glLineWidth(1.0f);
}

void drawString(float x, float y, const char *str, void *font)
{
    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void drawStringLarge(float x, float y, const char *str)
{
    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void drawStringSmall(float x, float y, const char *str)
{
    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawCountdownUI(float dt)
{
    int seconds = (int)ceilf(countdownTimer);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Dark semi-transparent overlay (leave top/bottom HUD bars clear)
    glColor4f(0.0f, 0.0f, 0.0f, 0.45f);
    fillRect(WL, WB + 14.0f, WR, WT - 9.0f);

    static float bannerPulse = 0;
    bannerPulse += dt * 7.0f;
    float bannerAlpha = 0.82f + sinf(bannerPulse) * 0.18f;

    float banH = 10.0f;
    float cY = 2.0f;
    float banW = 60.0f;
    float bx1 = -banW, bx2 = banW;
    glColor4f(0.72f, 0.0f, 0.0f, bannerAlpha);
    fillRect(bx1, cY - banH * 0.5f, bx2, cY + banH * 0.5f);

    glColor4f(1.0f, 0.4f, 0.0f, 1.0f);
    fillRect(bx1, cY + banH * 0.5f - 0.5f, bx2, cY + banH * 0.5f);
    fillRect(bx1, cY - banH * 0.5f, bx2, cY - banH * 0.5f + 0.5f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    if (gAttackType == AttackType::MISSILE)
        drawStringLarge(-25.5f, cY - 1.2f, "! INCOMING MISSILE STRIKE !");
    else if (gAttackType == AttackType::DRONE)
        drawStringLarge(-20.5f, cY - 1.2f, "! DRONE BOMB INBOUND !");
    else if (gAttackType == AttackType::EARTHQUAKE)
        drawStringLarge(-26.5f, cY - 1.2f, "! SEISMIC ALERT - EVACUATE !");

    if (seconds > 0)
    {
        glColor4f(1.0f, 0.92f, 0.0f, 1.0f);
        draw7SegmentDigit(0, cY + 14.0f, 14.0f, seconds);
    }

    static float warnT = 0;
    warnT += dt;
    float borderAlpha = sinf(warnT * 14.0f) * 0.45f + 0.45f;
    glColor4f(1.0f, 0.05f, 0.0f, borderAlpha);
    float bw = 4.0f;
    fillRect(WL, WB, WR, WB + bw);
    fillRect(WL, WT - bw, WR, WT);
    fillRect(WL, WB, WL + bw, WT);
    fillRect(WR - bw, WB, WR, WT);

    glColor4f(1.0f, 1.0f, 0.0f, 0.95f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    float cm = 9.0f;
    glVertex2f(WL, WT - cm);
    glVertex2f(WL, WT);
    glVertex2f(WL, WT);
    glVertex2f(WL + cm, WT);
    glVertex2f(WR - cm, WT);
    glVertex2f(WR, WT);
    glVertex2f(WR, WT);
    glVertex2f(WR, WT - cm);
    glVertex2f(WL, WB + cm);
    glVertex2f(WL, WB);
    glVertex2f(WL, WB);
    glVertex2f(WL + cm, WB);
    glVertex2f(WR - cm, WB);
    glVertex2f(WR, WB);
    glVertex2f(WR, WB);
    glVertex2f(WR, WB + cm);
    glEnd();
    glLineWidth(1.0f);
}

void drawLineDDA(float x1, float y1, float x2, float y2)
{
    float lw;
    glGetFloatv(GL_LINE_WIDTH, &lw);
    glPointSize(lw);

    float dx = x2 - x1;
    float dy = y2 - y1;
    // Scale up for resolution (OpenGL coordinate system uses floats, so we simulate pixel grid)
    float steps = std::max(std::abs(dx), std::abs(dy)) * 100.0f;
    if (steps < 1.0f)
        steps = 1.0f;

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= (int)steps; i++)
    {
        glVertex2f(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();
    glPointSize(1.0f);
}

void drawCircleMidpoint(float cx, float cy, float r, bool fill)
{

    float scale = 100.0f;
    int R = (int)(r * scale);
    int x = R, y = 0;
    int P = 1 - R;

    glBegin(fill ? GL_LINES : GL_POINTS);
    auto plot = [&](int x, int y)
    {
        float xf = x / scale;
        float yf = y / scale;
        if (!fill)
        {
            glVertex2f(cx + xf, cy + yf);
            glVertex2f(cx - xf, cy + yf);
            glVertex2f(cx + xf, cy - yf);
            glVertex2f(cx - xf, cy - yf);
            glVertex2f(cx + yf, cy + xf);
            glVertex2f(cx - yf, cy + xf);
            glVertex2f(cx + yf, cy - xf);
            glVertex2f(cx - yf, cy - xf);
        }
        else
        {
            glVertex2f(cx - xf, cy + yf);
            glVertex2f(cx + xf, cy + yf);
            glVertex2f(cx - xf, cy - yf);
            glVertex2f(cx + xf, cy - yf);
            glVertex2f(cx - yf, cy + xf);
            glVertex2f(cx + yf, cy + xf);
            glVertex2f(cx - yf, cy - xf);
            glVertex2f(cx + yf, cy - xf);
        }
    };

    while (x >= y)
    {
        plot(x, y);
        y++;
        if (P <= 0)
        {
            P = P + 2 * y + 1;
        }
        else
        {
            x--;
            P = P + 2 * y - 2 * x + 1;
        }
    }
    glEnd();
}
