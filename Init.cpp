#include "Init.h"
#include "Entity.h"
#include "ResourcesManager.h"
SDL_Texture* Init::enemyTex = nullptr;
SDL_Texture* Init::bullettex = nullptr;
SDL_Texture* Init::droneTex = nullptr;
SDL_Texture* Init::droneSkill = nullptr;
SDL_Texture* Init::healSkill = nullptr;
SDL_Texture* Init::menuTex = nullptr;
SDL_Texture* Init::gameOverTex = nullptr;
SDL_Texture* Init::fireBallTex = nullptr;
Mix_Chunk* Init::sounds[SND_MAX];
Entity Init::button[MAX_BUTTON];
HealthBar Init::playerHealthBar;
void Init::initGame()
{
    enemyTex = ResourcesManager::loadTexture("Texture/Demon.png");
    bullettex = ResourcesManager::loadTexture("Texture/bullet.png");
    droneTex = ResourcesManager::loadTexture("Texture/drone.png");
    healSkill = ResourcesManager::loadTexture("Texture/healCD.png");
    droneSkill = ResourcesManager::loadTexture("Texture/droneCD.png");
    menuTex = ResourcesManager::loadTexture("Texture/Menu.png");
    gameOverTex = ResourcesManager::loadTexture("Texture/GameOver.png");
    fireBallTex = ResourcesManager::loadTexture("Texture/fireBall.png");
}

void Init::initSound()
{
    sounds[SND_RIFLE_FIRE] = Mix_LoadWAV("Sounds/rifle_shoot.wav");
    sounds[SND_SWORD_SLASH] = Mix_LoadWAV("Sounds/sword_slash.wav");
    sounds[SND_DRONE_LAUNCH] = Mix_LoadWAV("Sounds/drone_launch.wav");
    sounds[SND_ENEMY_DIE] = Mix_LoadWAV("Sounds/enemy_dead.mp3");
    sounds[SND_PLAYER_HURT] = Mix_LoadWAV("Sounds/player_hurt.wav");
    sounds[SND_HEAL] = Mix_LoadWAV("Sounds/player_heal.wav");
    sounds[SND_BUTTON_CLICK] = Mix_LoadWAV("Sounds/button_click.wav");
    sounds[SND_ENEMY_FIRE] = Mix_LoadWAV("Sounds/enemy_fire.wav");
}

void Init::initMenu()
{
    //Play Button
    button[PLAY_BUTTON].tex = ResourcesManager::loadTexture("Texture/PlayButton.png");
    //Retry Button
    button[RETRY_BUTTON].tex = ResourcesManager::loadTexture("Texture/RetryButton.png");
    //Menu Button
    button[MENU_BUTTON].tex = ResourcesManager::loadTexture("Texture/MenuButton.png");
}

void Init::initEnemy(int x, int y)
{

    Entity* enemy;
    enemy = new Entity;
    memset(enemy, 0, sizeof(Entity));
    game::entityTail->next = enemy;
    game::entityTail = enemy;
    enemy->health = 1;
    enemy->radius = 32;
    enemy->tex = enemyTex;
    enemy->type = ENEMY;
    enemy->srcRect.x = enemy->srcRect.y = 0;
    enemy->srcRect.w = enemy->srcRect.h = 32;
    enemy->dstRect.h = 64;
    enemy->dstRect.w = 64;
    enemy->x = x; enemy->y = y;

}
void Init::createBullet(SDL_Texture* tex, int faction, float x, float y, float speed, SDL_Rect target, int doTracking, float trackingDelay)
{
    Entity* bullet;
    bullet = new Entity;
    memset(bullet, 0, sizeof(Entity));
    game::bulletTail->next = bullet;
    game::bulletTail = bullet;
    bullet->health = FPS * (doTracking == TRACKING ? 2.5 : 2);
    bullet->type = faction;
    bullet->radius = doTracking;
    bullet->energy = trackingDelay;
    bullet->reload = speed;
    bullet->x = x;
    bullet->y = y;
    if (faction == PLAYER)
    {
        bullet->dstRect.h = (doTracking == TRACKING ? 14 : 8);
        bullet->dstRect.w = (doTracking == TRACKING ? 7 : 3);
    }
    if (faction == ENEMY)
    {
        bullet->dstRect.h = 16;
        bullet->dstRect.w = 16;
    }
    Logic::findPath(bullet->x - game::camera.x, bullet->y - game::camera.y, target.x, target.y, bullet->dx, bullet->dy);
    bullet->tex = tex;
}

void Init::initHealthBar()
{
    playerHealthBar.healthbarTex = ResourcesManager::loadTexture("Texture/HealthBar.png");
    playerHealthBar.dstRect.h = 32;
    playerHealthBar.dstRect.w = 128;
    playerHealthBar.healthBarFrame.w = playerHealthBar.healthBar.w = playerHealthBar.energyBar.w = 32;
    playerHealthBar.healthBarFrame.h = playerHealthBar.healthBar.h = playerHealthBar.energyBar.h = 8;
    playerHealthBar.healthBarFrame.x = playerHealthBar.healthBarFrame.y = 0;
    playerHealthBar.energyBar.x = 64;
    playerHealthBar.healthBar.x = 32;
}
