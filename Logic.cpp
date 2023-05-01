#include "Logic.h"

SDL_Event Logic::event;
int Logic::mouseState;
int Logic::enemySpawnTimer;
//Ultilities
double Logic::getDist(float x1, float y1, float x2, float y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double Logic::getAngle(float x1, float y1, float x2, float y2)
{
    float angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / 3.14);
    return angle >= 0 ? angle : 360 + angle;
}

void Logic::findPath(float x1, float y1, float x2, float y2, float& dx, float& dy)
{
    int steps = std::max(abs(x1 - x2), abs(y1 - y2));
    if (steps == 0)
    {
        dx = dy = 0;
    }
    else
    {
        dx = (x2 - x1) / steps;
        dy = (y2 - y1) / steps;
    }
}

void Logic::spawnEnemy()
{
    int x, y;

    if (--enemySpawnTimer <= 0)
    {
        x = game::camera.x;
        y = game::camera.y;
        switch (rand() % 4)
        {
        case 0:
            x += -100;
            y += rand() % SCREEN_HEIGHT;
            break;

        case 1:
            x += SCREEN_WIDTH + 100;
            y += rand() % SCREEN_HEIGHT;
            break;

        case 2:
            x += rand() % SCREEN_WIDTH;
            y += -100;
            break;

        case 3:
            x += rand() % SCREEN_WIDTH;
            y += SCREEN_HEIGHT + 100;
            break;
        }

        Init::initEnemy(x, y);

        enemySpawnTimer = ENEMY_SPAWN_TIMER - (game::score / 50) * FPS / float(4);
    }
}


void Logic::buttonVisual(Entity& button, float x, float y)
{
    if (SDL_HasIntersection(&game::mouseRect, &button.dstRect))
    {
        button.dstRect.h = BUTTON_HEIGHT * 1.2;
        button.dstRect.w = BUTTON_WIDTH * 1.2;
    }
    else
    {
        button.dstRect.h = BUTTON_HEIGHT;
        button.dstRect.w = BUTTON_WIDTH;
    }
    button.dstRect.x = x - button.dstRect.w / 2;
    button.dstRect.y = y - button.dstRect.h / 2;
}

//Main logic


void Logic::doMenu()
{
    if (!game::isPlaying)
    {
        if (game::inMainMenu)
        {
            //Visual
            buttonVisual(Init::button[PLAY_BUTTON], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
            //Click
            if (mouseState && SDL_HasIntersection(&game::mouseRect, &Init::button[PLAY_BUTTON].dstRect))
            {
                ResourcesManager::playSFX(SND_BUTTON_CLICK, CH_SYSTEM);
                game::clearStage();
                game::initStage();
                game::inMainMenu = false;
                mouseState = 0;
            }
        }
        else
        {
            //Visual
            buttonVisual(Init::button[RETRY_BUTTON], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);
            buttonVisual(Init::button[MENU_BUTTON], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 + 40 + BUTTON_HEIGHT);
            //Click
            if (mouseState && SDL_HasIntersection(&game::mouseRect, &Init::button[RETRY_BUTTON].dstRect))
            {
                ResourcesManager::playSFX(SND_BUTTON_CLICK, CH_SYSTEM);
                game::clearStage();
                game::initStage();
                mouseState = 0;
            }
            if (mouseState && SDL_HasIntersection(&game::mouseRect, &Init::button[MENU_BUTTON].dstRect))
            {
                ResourcesManager::playSFX(SND_BUTTON_CLICK, CH_SYSTEM);
                game::clearStage();
                game::inMainMenu = true;
                mouseState = 0;
            }
        }
    }
}


void Logic::doEntity()
{
    Entity* prev, * obj;
    prev = &game::entityHead;
    for (obj = game::entityHead.next; obj != NULL; obj = obj->next)
    {
        switch (obj->type)
        {
        case ENEMY:
            if (Player::player != NULL)
            {
                obj->reload--;
                if (obj->reload <= 0)
                    obj->reload = 0;
                obj->dx = obj->dy = 0;
                obj->texrotate = SDL_FLIP_NONE;
                if (getDist(obj->x + 32, obj->y + 32, Player::player->x + 32, Player::player->y + 32) >= 240)
                {
                    findPath(obj->x, obj->y, Player::player->x, Player::player->y, obj->dx, obj->dy);
                }
                obj->dx *= 3;
                obj->dy *= 3;
                if (getAngle(obj->x, obj->y, Player::player->x, Player::player->y) < 180)
                {
                    obj->texrotate = SDL_FLIP_HORIZONTAL;
                }
                if (getDist(Player::player->x - 32, Player::player->y - 32, obj->x - 32, obj->y - 32) <= 240 && obj->reload <= 0)
                {
                    Init::createBullet(Init::fireBallTex, ENEMY, obj->x, obj->y, FIREBALL_SPEED, Render::cameraAdjust(Player::player->dstRect, Player::player->x + 32, Player::player->y + 32), NOTRACKING, 0);
                    obj->reload = FPS * 1.5;
                    ResourcesManager::playSFX(SND_ENEMY_FIRE, CH_ANY);
                }
            }
        }
        //General logic
        //Map bound and Collision
        int mx, my, mx_, my_;
        mx = (obj->x + obj->dx + (obj->dx <= 0 ? 0 : obj->dstRect.w)) / TILE_SIZE;
        my = (obj->y + obj->dy + (obj->dy <= 0 ? 0 : obj->dstRect.h)) / TILE_SIZE;
        mx_ = (obj->x) / TILE_SIZE;
        my_ = (obj->y) / TILE_SIZE;
        if (game::newmap->gmap[mx_][my] == 2 || game::newmap->gmap[mx_ + 1][my] == 2)
            obj->dy = 0;
        if (game::newmap->gmap[mx][my_] == 2 || game::newmap->gmap[mx][my_ + 1] == 2 || game::newmap->gmap[mx][my_ + 2] == 2)
            obj->dx = 0;
        //Entity Collision
        SDL_Rect temp;
        for (Entity* e = game::entityHead.next; e != NULL; e = e->next)
        {
            temp = obj->dstRect;
            if (e != obj)
            {
                temp.x += obj->dx;
                if (getDist(temp.x + 32, temp.y + 32, e->dstRect.x + 32, e->dstRect.y + 32) <= 30)
                {
                    obj->dx = 0;
                }
                temp.x -= obj->dx;
                temp.y += obj->dy;
                if (getDist(temp.x + 32, temp.y + 32, e->dstRect.x + 32, e->dstRect.y + 32) <= 30)
                {
                    obj->dy = 0;
                }
            }
        }
        obj->x += obj->dx;
        obj->y += obj->dy;
        //Animation
        if ((obj->dy != 0 || obj->dx != 0) && --obj->delay <= 0)
        {
            obj->srcRect.x += 32;
            obj->delay = FPS * 0.1;
        }
        if (obj->dy == 0 && obj->dx == 0)
        {
            obj->srcRect.x = 0;
        }
        if (obj->srcRect.x > 160)
        {
            obj->srcRect.x = 0;
        }

        //Delete object
        if (obj->health <= 0)
        {
            if (obj->type == ENEMY)
            {
                game::score++;
                if (obj->x - game::camera.x < SCREEN_WIDTH && obj->y - game::camera.y < SCREEN_HEIGHT && obj->x - game::camera.x > 0 && obj->y - game::camera.y > 0)
                {
                    ResourcesManager::playSFX(SND_ENEMY_DIE, CH_ENEMY);
                }
            }

            if (obj == game::entityTail)
            {
                game::entityTail = prev;
            }
            prev->next = obj->next;
            if (obj == Player::player)
                Player::player = NULL;
            delete obj;
            obj = prev;
        }
        prev = obj;
    };
}

void Logic::doInput()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            game::isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.repeat == 0 && event.key.keysym.scancode < MAX_KEYBOARD_KEYS)
                game::key[event.key.keysym.scancode] = 1;
            break;
        case SDL_KEYUP:
            if (event.key.repeat == 0 && event.key.keysym.scancode < MAX_KEYBOARD_KEYS)
                game::key[event.key.keysym.scancode] = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseState = 1;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseState = 0;
        default:
            break;
        }
    }
}
void Logic::doCamera()
{
    if (Player::player != 0)
    {
        game::camera.x = Player::player->x - SCREEN_WIDTH / 2;
        game::camera.y = Player::player->y - SCREEN_HEIGHT / 2;
        game::camera.x = std::min(std::max(game::camera.x, 0), (ARENA_WIDTH * TILE_SIZE) - SCREEN_WIDTH);
        game::camera.y = std::min(std::max(game::camera.y, 0), (ARENA_HEIGHT * TILE_SIZE) - SCREEN_HEIGHT);
    }
}

void Logic::doBullet()
{
    Entity* bullet, * prev;
    prev = &game::bulletHead;
    for (bullet = game::bulletHead.next; bullet != NULL; bullet = bullet->next)
    {
        int mx = bullet->x, my = bullet->y;
        if (bullet->energy > 0)
        {
            bullet->energy--;
        }

        if (bullet->radius == TRACKING && bullet->energy <= 0)
        {
            Entity* obj, * target;
            target = &game::entityHead;
            for (obj = game::entityHead.next; obj != NULL; obj = obj->next)
            {
                if (Logic::getDist(bullet->x, bullet->y, obj->x, obj->y) < Logic::getDist(bullet->x, bullet->y, target->x, target->y) && obj->type == ENEMY)
                {
                    target = obj;
                }
            }
            if (Logic::getDist(bullet->x, bullet->y, target->x, target->y) <= 250)
            {
                Logic::findPath(bullet->x - game::camera.x, bullet->y - game::camera.y, target->x - game::camera.x, target->y - game::camera.y, bullet->dx, bullet->dy);

            }

        }
        bullet->x += bullet->dx * bullet->reload;
        bullet->y += bullet->dy * bullet->reload;
        bullet->angle = Logic::getAngle(mx - game::camera.x, my - game::camera.y, bullet->x - game::camera.x, bullet->y - game::camera.y);
        if (game::newmap->gmap[int(bullet->x / TILE_SIZE)][int(bullet->y / TILE_SIZE)] == 2)
        {
            bullet->health = 0;
        }
        else
        {
            for (Entity* obj = game::entityHead.next; obj != 0; obj = obj->next)
            {
                if (SDL_HasIntersection(&bullet->dstRect, &obj->dstRect) && obj->type != bullet->type)
                {
                    obj->health -= 50;
                    bullet->health = 0;
                    if (obj->type == PLAYER)
                    {
                        ResourcesManager::playSFX(SND_PLAYER_HURT, CH_ANY);
                    }
                }
            }
        }
        if (bullet->health <= 0)
        {
            if (bullet == game::bulletTail)
            {
                game::bulletTail = prev;
            }
            prev->next = bullet->next;
            delete bullet;
            bullet = prev;
        }
        prev = bullet;
        bullet->health--;
    }
}


void Logic::doHealthBar()
{
    if (Player::player != NULL)
    {
        Init::playerHealthBar.healthBarR = Player::player->health * float(128) / PLAYER_MAX_HEALTH;
        if (Init::playerHealthBar.healthBarR < 0)
        {
            Init::playerHealthBar.healthBarR = 0;
        }
        Init::playerHealthBar.energyBarR = Player::player->energy * float(128) / PLAYER_MAX_ENERGY;
        if (Init::playerHealthBar.energyBarR < 0)
        {
            Init::playerHealthBar.energyBarR = 0;
        }
        Init::playerHealthBar.dstRect.x = 0;
        Init::playerHealthBar.dstRect.y = SCREEN_HEIGHT - Init::playerHealthBar.dstRect.h;
    }
}
