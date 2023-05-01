#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "game.h"

class ResourcesManager
{
public:
    static SDL_Texture* loadTexture(const char* filePath);
    static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst);
    static void playSFX(int ID, int channel);

};

#endif // TEXTUREMANAGER_H
