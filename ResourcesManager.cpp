#include "ResourcesManager.h"

SDL_Texture* ResourcesManager::loadTexture(const char* filePath)
{
    SDL_Surface* temp = IMG_Load(filePath);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(game::Renderer, temp);
    SDL_FreeSurface(temp);
    return tex;
}
void ResourcesManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst)
{
    SDL_RenderCopy(game::Renderer, tex, &src, &dst);
}
void ResourcesManager::playSFX(int ID, int channel)
{
    Mix_PlayChannel(channel, Init::sounds[ID], 0);
}
