#include "game.h"
//Game
game::game()
{
    //ctor
}

game::~game()
{
    //dtor
}


//Initialize global value
int game::key[MAX_KEYBOARD_KEYS];
int game::score;
SDL_Renderer* game::Renderer = nullptr;
Entity game::entityHead;
Entity* game::entityTail = nullptr;
Entity game::bulletHead;
Entity* game::bulletTail = nullptr;
SDL_Rect game::mouseRect;
SDL_Point game::camera;
Map* game::newmap = nullptr;
bool game::isRunning;
bool game::isPlaying = false;
bool game::inMainMenu;
//Game logic
void game::initGame(const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL initialized" << std::endl;
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == 0)
        {
            std::cout << "SDL sound initialized" << std::endl;
        }
        Mix_AllocateChannels(MAX_CHANNEL);
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        Renderer = SDL_CreateRenderer(window, -1, 0);
        isRunning = true;
        SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 0);
        Init::initGame();
        Init::initMenu();
        Init::initSound();
        Render::initWordChart();
        //Initialize mouse
        mouseTexture = ResourcesManager::loadTexture("Texture/mouse.png");
        mouseRect.w = mouseRect.h = 16;
        SDL_ShowCursor(0);
        inMainMenu = true;
    }
    else
        isRunning = false;
}

void game::initStage()
{
    score = 0;
    //Initialize Object
    entityTail = &entityHead;
    bulletTail = &bulletHead;
    Player::initPlayer();
    Player::initWeapon("Rifle", RIFLE, "Texture/Rifle.png");
    Player::initWeapon("Sword", SWORD, "Texture/sword.png");
    Init::initHealthBar();
    //Initialize Map
    newmap = new Map;
    newmap->loadMap("Data/arena.dat");
    isPlaying = true;
}

void game::clearStage()
{
    Entity* obj;

    while (entityHead.next)
    {
        obj = entityHead.next;
        entityHead.next = obj->next;
        delete obj;
    }

    while (bulletHead.next)
    {
        obj = bulletHead.next;
        bulletHead.next = obj->next;
        delete obj;
    }
    delete newmap;
    isPlaying = false;
}

void game::handleInput()
{
    Logic::doInput();
    SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
}

void game::updateGame()
{
    mouseRect.x -= mouseRect.w / 2;
    mouseRect.y -= mouseRect.h / 2;
    if (isPlaying)
    {
        Logic::doCamera();
        Logic::spawnEnemy();
        Player::doPlayer();
        Logic::doEntity();
        Player::doWeapon();
        Logic::doBullet();
        Logic::doHealthBar();
        if (Player::player == NULL) { isPlaying = false; }
    }
    //Menu handling
    Logic::doMenu();
}

void game::renderGame()
{
    SDL_RenderClear(Renderer);
    if (isPlaying)
    {
        newmap->drawMap();
        Render::renderObject();
        Render::renderBullet();
        Render::renderweapon();
        Render::renderHealthBar();
        Render::renderGUI();
    }
    Render::renderMenu();
    SDL_RenderCopy(Renderer, mouseTexture, NULL, &mouseRect);
    SDL_RenderPresent(Renderer);
}


void game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Renderer);
    SDL_Quit();
}
