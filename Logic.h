#ifndef LOGIC_H
#define LOGIC_H
#include "game.h"
#include <cmath>
class Logic
{
public:
    static double getDist(float x1, float y1, float x2, float y2);
    static void doInput();
    static double getAngle(float x1, float y1, float x2, float y2);
    static void spawnEnemy();
    static void doEntity();
    static void findPath(float x1, float y1, float x2, float y2, float& dx, float& dy);
    static int enemySpawnTimer;
    static void doCamera();
    static void doBullet();
    static void doHealthBar();
    static void doMenu();
    static int mouseState;
private:
    static SDL_Event event;
    static void buttonVisual(Entity& button, float x, float y);
};

#endif // LOGIC_H
