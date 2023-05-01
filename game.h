#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "define.h"
#include "Map.h"
#include "Entity.h"
#include "ResourcesManager.h"
#include "Init.h"
#include "Logic.h"
#include "Render.h"
#include "Player.h"
#include <time.h>
class Map;
class game
{
public:
    game();
    virtual ~game();
    void initGame(const char* title, int width, int height);
    void initTileScreen();
    static void initStage();
    static void clearStage();
    void handleInput();
    void renderGame();
    void updateGame();
    void clean();
    static SDL_Renderer* Renderer;
    static SDL_Point camera;
    //Global value
    static int key[MAX_KEYBOARD_KEYS];
    static bool isRunning;
    static Entity* entityTail, * bulletTail;
    static Entity entityHead, bulletHead;
    static SDL_Rect mouseRect;
    static Map* newmap;
    static int score;
    static bool isPlaying;
    static bool inMainMenu;
private:
    SDL_Window* window;
    //Mouse data
    SDL_Texture* mouseTexture;;

};


#endif // GAME_H
