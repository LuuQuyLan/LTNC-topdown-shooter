#ifndef PLAYER_H
#define PLAYER_H
#include "game.h"

class Player
{
public:
    static int currentWeaponID;
    static Entity* currentWeapon;
    static void initWeapon(const char* name, int weaponID, const char* filePath);
    static void doWeapon();
    static Entity* player;
    static void doPlayer();
    static void initPlayer();
    static int attackFrame;
    static float heal_Cooldown, drone_Cooldown;
private:
    static Entity weapon[MAX_WEAPON];

};

#endif // PLAYER_H
