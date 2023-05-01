#include "game.h"
#include "define.h"
game* newGame = nullptr;
const int frameDelay = 1000 / FPS;
Uint32 frameStart;
int frameTime;

int main(int argc, char* args[])
{
    newGame = new game;
    newGame->initGame("Prototype 01", SCREEN_WIDTH, SCREEN_HEIGHT);
    srand(time(NULL));
    while (newGame->isRunning)
    {
        frameStart = SDL_GetTicks();
        newGame->handleInput();
        newGame->updateGame();
        newGame->renderGame();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    newGame->clean();
    return 0;
}
