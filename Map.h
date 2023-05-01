#ifndef MAP_H
#define MAP_H
#include"game.h"

class Map
{
public:
    Map();
    ~Map();


    void loadMap(const char* filePath);
    void drawMap();
    int gmap[ARENA_WIDTH][ARENA_HEIGHT];
private:

    SDL_Rect src, dest;
    SDL_Texture* floor;
    SDL_Texture* floor1;
    SDL_Texture* wall;

};

#endif // MAP_H
