#ifndef ENTITY_H
#define ENTITY_H
#include "game.h"
struct Entity
{
    const char* name;
    float x, y, dx, dy, radius, health, energy, reload, delay;
    SDL_Rect srcRect, dstRect;
    SDL_Texture* tex;
    Entity* next;
    int type;
    double angle;
    SDL_RendererFlip texrotate;
    SDL_Point center;
};



#endif // ENTITY_H
