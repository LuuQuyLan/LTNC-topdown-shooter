#ifndef INIT_H
#define INIT_H
#include "game.h"

struct HealthBar
{
    SDL_Texture* healthbarTex;
    SDL_Rect dstRect;
    SDL_Rect healthBarFrame;
    SDL_Rect healthBar;
    SDL_Rect energyBar;
    float healthBarR, energyBarR;
};

class Init
{
public:
    static void initEnemy(int x, int y);
    static void initGame();
    static void createBullet(SDL_Texture* tex, int faction, float x, float y, float speed, SDL_Rect target, int doTracking, float trackingDelay);
    static void initHealthBar();
    static HealthBar playerHealthBar;
    static SDL_Texture* bullettex;
    static SDL_Texture* droneTex;
    static SDL_Texture* fireBallTex;
    static SDL_Texture* healSkill, * droneSkill;
    static void initMenu();
    static Entity button[MAX_BUTTON];
    static SDL_Texture* menuTex, * gameOverTex;
    static void initSound();
    static Mix_Chunk* sounds[SND_MAX];
private:
    static SDL_Texture* enemyTex;
};


#endif // INIT_H
