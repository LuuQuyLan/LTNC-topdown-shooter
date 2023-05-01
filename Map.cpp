#include "Map.h"
#include "ResourcesManager.h"
#include <fstream>
Map::Map()
{
    floor = ResourcesManager::loadTexture("Texture/floor.png");
    floor1 = ResourcesManager::loadTexture("Texture/floor1.png");
    wall = ResourcesManager::loadTexture("Texture/wall.png");
    src.x = src.y = 0;
    src.w = dest.w = TILE_SIZE;
    src.h = dest.h = TILE_SIZE;
    dest.x = dest.y = 0;
}

Map:: ~Map()
{
    delete gmap;
}

void Map::loadMap(const char* filePath)
{
    std::ifstream fileIn;
    fileIn.open(filePath);
    for (int row = 0; row < ARENA_HEIGHT; row++)
        for (int column = 0; column < ARENA_WIDTH; column++)
        {
            fileIn >> gmap[column][row];
        }
    fileIn.close();
}

void Map::drawMap()
{
    int x, y, n, x1, x2, y1, y2, mx, my;

    x1 = -(game::camera.x % TILE_SIZE);
    x2 = x1 + ARENA_RENDER_WIDTH * TILE_SIZE + (x1 == 0 ? 0 : TILE_SIZE);

    y1 = -(game::camera.y % TILE_SIZE);
    y2 = y1 + ARENA_RENDER_HEIGHT * TILE_SIZE + (y1 == 0 ? 0 : TILE_SIZE);

    mx = game::camera.x / TILE_SIZE;
    my = game::camera.y / TILE_SIZE;
    for (y = y1; y < y2; y += TILE_SIZE)
    {
        for (x = x1; x < x2; x += TILE_SIZE)
        {
            dest.x = x;
            dest.y = y;
            if (mx >= 0 && my >= 0 && mx < ARENA_WIDTH && my < ARENA_HEIGHT)
            {
                n = gmap[mx][my];
                switch (n)
                {
                case 0:
                    SDL_RenderCopy(game::Renderer, floor, &src, &dest);
                    break;
                case 1:
                    SDL_RenderCopy(game::Renderer, floor1, &src, &dest);
                    break;
                case 2:
                    SDL_RenderCopy(game::Renderer, wall, &src, &dest);
                    break;
                default:
                    break;
                }
            }

            mx++;
        }

        mx = game::camera.x / TILE_SIZE;

        my++;
    }
}


