#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

void fillRect(float l,float b,float r,float t);
GLuint loadTexture(const char* path, bool flip=true);
void drawScene(GLuint texID);
void drawSprite(GLuint tex, float x, float y, float w, float h, float angle, bool flipX=false);

void draw7SegmentDigit(float x, float y, float size, int num);
void drawString(float x, float y, const char* str, void* font = GLUT_BITMAP_HELVETICA_18);
void drawStringLarge(float x, float y, const char* str);
void drawStringSmall(float x, float y, const char* str);
void drawCountdownUI(float dt);

// Custom graphics algorithms
void drawLineDDA(float x1, float y1, float x2, float y2);
void drawCircleMidpoint(float cx, float cy, float r, bool fill = true);

#endif // GRAPHICS_H
