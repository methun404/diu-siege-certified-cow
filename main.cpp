#include <GLFW/glfw3.h>
#include <iostream>

#include "config.h"
#include "state.h"
#include "audio.h"
#include "graphics.h"
#include "entities.h"
#include "game.h"

void keyCallback(GLFWwindow *win, int key, int, int action, int)
{
    if (key >= 0 && key < 1024)
    {
        keys[key] = (action != GLFW_RELEASE);
    }

    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_C)
    {
        if (!playerCowActive && gState == State::IDLE)
        {
            playerCowActive = true;
            playerCow.x = 0.0f;
            playerCow.y = -25.0f;
            playerCow.speed = 10.0f;
            playerCow.state = CowState::WANDER;
            playerCow.dir = 1;
            playerCow.animOffset = 0.0f;
            playerCow.vx = 0;
            playerCow.vy = 0;
            playerCow.angle = 0.0f;
            playSound(sndCowMoo, 0.85f);
        }
        return;
    }

    if (key == GLFW_KEY_G && playerCowActive && gState == State::IDLE)
    {
        playSound(sndCowMoo, 0.6f);
    }

    if (key == GLFW_KEY_SPACE)
    {
        resetScene();
        return;
    }

    if (key == GLFW_KEY_S)
    {
        musicMuted = !musicMuted;
        if (sndTheme.loaded)
        {
            ma_sound_set_volume(&sndTheme.snd, musicMuted ? 0.0f : 0.22f);
        }
        return;
    }

    if (gState == State::IDLE)
    {
        if (key == GLFW_KEY_M)
            triggerAttack(AttackType::MISSILE);
        else if (key == GLFW_KEY_D)
            triggerAttack(AttackType::DRONE);
        else if (key == GLFW_KEY_E)
            triggerAttack(AttackType::EARTHQUAKE);
    }
    else if (gState == State::DESTROYED)
    {
        if (key == GLFW_KEY_M || key == GLFW_KEY_D || key == GLFW_KEY_E)
        {
            resetScene();
        }
    }

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(win, GLFW_TRUE);
}

int main()
{
    srand((unsigned)time(nullptr));
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    GLFWwindow *win = glfwCreateWindow(WIN_W, WIN_H,
                                       "CG Lab - REALISTIC Strikes  |  M = Missile  |  D = Drone  |  E = Earthquake  |  ESC = Quit",
                                       nullptr, nullptr);
    if (!win)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, keyCallback);
    glfwSwapInterval(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(WL, WR, WB, WT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    initAudio();
    texSceneBefore = loadTexture("images/scene_before.png", true);
    texSceneAfterMissile = loadTexture("images/scene_after_missile.png", true);
    texSceneAfterDrone = loadTexture("images/scene_after_drone.png", true);
    texSceneAfterEarthquake = loadTexture("images/scene_after_earthquake.png", true);

    texMissileSprite = loadTexture("images/missile_sprite.png", true);
    texDroneSprite = loadTexture("images/drone_sprite.png", true);
    texBombSprite = loadTexture("images/bomb_sprite.png", true);
    texParticle = loadTexture("images/particle.png", false);
    texNPCSprite = loadTexture("images/npc_sprite.png", true);

    initNPCs();

    double prev = glfwGetTime();
    while (!glfwWindowShouldClose(win))
    {
        double now = glfwGetTime();
        float dt = (float)(now - prev);
        prev = now;
        if (dt > 0.05f)
            dt = 0.05f;
        update(dt);
        render(dt);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    cleanupAudio();
    return 0;
}
