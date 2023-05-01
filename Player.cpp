#include "Player.h"
Entity* Player::player = nullptr;
int Player::attackFrame;
Entity* Player::currentWeapon = nullptr;
Entity Player::weapon[MAX_WEAPON];
int Player::currentWeaponID;
float Player::heal_Cooldown, Player::drone_Cooldown;


void Player::initPlayer()
{
    player = new Entity;
    memset(player, 0, sizeof(Entity));
    game::entityTail->next = player;
    game::entityTail = player;
    player->tex = ResourcesManager::loadTexture("Texture/player.png");
    player->type = PLAYER;
    player->health = PLAYER_MAX_HEALTH;
    player->energy = PLAYER_MAX_ENERGY;
    player->srcRect.x = player->srcRect.y = 0;
    player->srcRect.w = player->srcRect.h = 32;
    player->dstRect.h = 64;
    player->dstRect.w = 64;
    player->radius = 32;
    player->x = 600;
    player->y = 400;
    player->delay = 0;
    heal_Cooldown = 0;
    drone_Cooldown = 0;
}

void Player::doPlayer()
{
    if (player != NULL)
    {
        if (--heal_Cooldown < 0) { heal_Cooldown = 0; }
        if (--drone_Cooldown < 0) { drone_Cooldown = 0; }

        player->dx = player->dy = 0;

        //Character Control
        player->texrotate = SDL_FLIP_NONE;
        if (game::key[SDL_SCANCODE_A])
            player->dx = -PLAYER_SPEED;
        if (game::key[SDL_SCANCODE_D])
            player->dx = PLAYER_SPEED;
        if (game::key[SDL_SCANCODE_W])
            player->dy = -PLAYER_SPEED;
        if (game::key[SDL_SCANCODE_S])
            player->dy = PLAYER_SPEED;
        if (game::key[SDL_SCANCODE_Q])
        {
            currentWeaponID--;
            if (currentWeaponID < 0)
                currentWeaponID = (MAX_WEAPON - 1);
            currentWeapon = &weapon[currentWeaponID];
            game::key[SDL_SCANCODE_Q] = 0;
        }
        if (game::key[SDL_SCANCODE_E])
        {
            currentWeaponID++;
            if (currentWeaponID > MAX_WEAPON - 1)
                currentWeaponID = 0;
            currentWeapon = &weapon[currentWeaponID];
            game::key[SDL_SCANCODE_E] = 0;
        }

        if (Logic::mouseState)
        {
            if (attackFrame <= 0)
            {
                attackFrame = FPS * 0.3;
                if (currentWeapon == &weapon[SWORD])
                {
                    ResourcesManager::playSFX(SND_SWORD_SLASH, CH_PLAYER);
                }
            }
        }


        //Heal Skill
        if (game::key[SDL_SCANCODE_F] && heal_Cooldown <= 0 && player->energy >= 300 && player->health < PLAYER_MAX_HEALTH)
        {
            ResourcesManager::playSFX(SND_HEAL, CH_ANY);
            player->health += 400;
            if (player->health > PLAYER_MAX_HEALTH) { player->health = PLAYER_MAX_HEALTH; }
            player->energy -= 300;
            heal_Cooldown = FPS * 30;

        }

        //Drone Skill
        if (game::key[SDL_SCANCODE_R] && drone_Cooldown <= 0 && player->energy >= 200)
        {
            //Sound Effect
            ResourcesManager::playSFX(SND_DRONE_LAUNCH, CH_ANY);
            //Mechanic
            SDL_Rect target{};
            target.x = 0;
            target.y = 0;
            Init::createBullet(Init::droneTex, PLAYER, player->x, player->y, DRONE_SPEED, target, TRACKING, FPS * 0.3);
            target.x = SCREEN_WIDTH;
            Init::createBullet(Init::droneTex, PLAYER, player->x, player->y, DRONE_SPEED, target, TRACKING, FPS * 0.3);
            target.x = 0;
            target.y = SCREEN_HEIGHT;
            Init::createBullet(Init::droneTex, PLAYER, player->x, player->y, DRONE_SPEED, target, TRACKING, FPS * 0.3);
            target.x = SCREEN_WIDTH;
            Init::createBullet(Init::droneTex, PLAYER, player->x, player->y, DRONE_SPEED, target, TRACKING, FPS * 0.3);
            player->energy -= 200;
            drone_Cooldown = FPS * 15;
        }

        //Flip to mouse
        if (Logic::getAngle(player->x - game::camera.x, player->y - game::camera.y, game::mouseRect.x, game::mouseRect.y) > 180)
        {
            player->texrotate = SDL_FLIP_HORIZONTAL;
        }
        //Map bound
        player->x = std::min(std::max(player->x, float(0)), float((ARENA_WIDTH * TILE_SIZE) - player->dstRect.w));
        player->y = std::min(std::max(player->y, float(0)), float((ARENA_HEIGHT * TILE_SIZE) - player->dstRect.h));
        //Energy Regen
        player->energy += 0.75;
        if (player->energy > PLAYER_MAX_ENERGY)
        {
            player->energy = PLAYER_MAX_ENERGY;
        }

    }
}
void Player::initWeapon(const char* name, int weaponID, const char* filePath)
{
    weapon[weaponID].name = name;
    weapon[weaponID].tex = ResourcesManager::loadTexture(filePath);
    weapon[weaponID].health = 1;
    weapon[weaponID].dstRect.h = weapon[weaponID].dstRect.w = 36;
    weapon[weaponID].radius = 12;
    weapon[weaponID].type = weaponID;
    currentWeaponID = weaponID;
    currentWeapon = &weapon[currentWeaponID];
}
void Player::doWeapon()
{
    if (player != NULL)
    {
        currentWeapon->texrotate = player->texrotate;
        currentWeapon->y = player->y + 8;
        currentWeapon->x = player->x + (currentWeapon->texrotate == SDL_FLIP_NONE ? 28 : 0);
        currentWeapon->center.y = 16;
        currentWeapon->center.x = 0 + (currentWeapon->texrotate == SDL_FLIP_NONE ? 2 : 34);
        if (attackFrame <= 0 || currentWeapon->type != SWORD)
        {
            currentWeapon->angle = (currentWeapon->texrotate == SDL_FLIP_NONE ? Logic::getAngle(currentWeapon->x - game::camera.x, currentWeapon->y - game::camera.y, game::mouseRect.x, game::mouseRect.y) :
                Logic::getAngle(game::mouseRect.x, game::mouseRect.y, currentWeapon->x - game::camera.x, currentWeapon->y - game::camera.y)) - 90;
        }
        else
        {
            currentWeapon->angle = (currentWeapon->texrotate == SDL_FLIP_NONE ? -12 * attackFrame + 120 : 12 * attackFrame - 120);
            attackFrame--;
            for (Entity* e = game::entityHead.next; e != NULL; e = e->next)
            {
                if (Logic::getDist(currentWeapon->x, currentWeapon->y, e->x, e->y) <= currentWeapon->radius + e->radius && e->type == ENEMY)
                {
                    e->health--;
                    player->energy += 30;
                }
            }
            for (Entity* b = game::bulletHead.next; b != NULL; b = b->next)
            {
                if (Logic::getDist(currentWeapon->x, currentWeapon->y, b->x, b->y) <= currentWeapon->radius + 16 && b->type == ENEMY)
                {
                    b->health = 0;
                }
            }
        }
        if (attackFrame > 0 && currentWeapon->type != SWORD)
        {
            if (currentWeapon->reload <= 0 && player->energy >= 30)
            {
                Init::createBullet(Init::bullettex, PLAYER, currentWeapon->x + (currentWeapon->texrotate == SDL_FLIP_NONE ? 2 : 34), currentWeapon->y + 16, BULLET_SPEED, game::mouseRect, NOTRACKING, 0);
                currentWeapon->reload = FPS * 0.3;
                player->energy -= 60;
                ResourcesManager::playSFX(SND_RIFLE_FIRE, CH_PLAYER);
            }
            attackFrame = 0;
        }
        else
            currentWeapon->reload--;
    }
}

