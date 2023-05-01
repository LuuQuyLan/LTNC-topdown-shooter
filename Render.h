#ifndef RENDER_H
#define RENDER_H
#include "game.h"
#include <string>
#include <iomanip>
class Render
{
public:
    static void renderGUI();
    static void renderText(int posX, int posY, std::string text, float charSize, int R, int G, int B);
    static void renderObject();
    static void renderweapon();
    static void renderBullet();
    static void renderHealthBar();
    static SDL_Rect cameraAdjust(SDL_Rect& rect, float x, float y);
    static void initWordChart();
    static void renderMenu();
private:
    static SDL_Texture* wordChart;
};

#endif // RENDER_H
