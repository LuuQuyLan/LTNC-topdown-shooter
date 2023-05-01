#include "Render.h"
SDL_Texture* Render::wordChart;
void Render::initWordChart()
{
    wordChart = ResourcesManager::loadTexture("Texture/font.png");
}
SDL_Rect Render::cameraAdjust(SDL_Rect& rect, float x, float y)
{
    rect.x = x;
    rect.y = y;
    SDL_Rect Tmp = rect;
    Tmp.x -= game::camera.x;
    Tmp.y -= game::camera.y;
    return Tmp;
}
void Render::renderObject()
{
    Entity* obj;
    for (obj = game::entityHead.next; obj != NULL; obj = obj->next)
    {
        SDL_Rect rect = cameraAdjust(obj->dstRect, obj->x, obj->y);
        SDL_RenderCopyEx(game::Renderer, obj->tex, &obj->srcRect, &rect, obj->angle, &obj->center, obj->texrotate);
    }
}
void Render::renderBullet()
{
    Entity* bullet;
    for (bullet = game::bulletHead.next; bullet != NULL; bullet = bullet->next)
    {
        SDL_Rect rect = cameraAdjust(bullet->dstRect, bullet->x, bullet->y);
        SDL_RenderCopyEx(game::Renderer, bullet->tex, NULL, &rect, bullet->angle, &bullet->center, bullet->texrotate);
    }
}
void Render::renderweapon()
{
    if (Player::player != NULL)
    {
        SDL_Rect rect = cameraAdjust(Player::currentWeapon->dstRect, Player::currentWeapon->x, Player::currentWeapon->y);
        SDL_RenderCopyEx(game::Renderer, Player::currentWeapon->tex, NULL, &rect, Player::currentWeapon->angle, &Player::currentWeapon->center, Player::currentWeapon->texrotate);
    }
}

void Render::renderHealthBar()
{
    SDL_Rect temp = Init::playerHealthBar.dstRect;
    SDL_RenderCopy(game::Renderer, Init::playerHealthBar.healthbarTex, &Init::playerHealthBar.healthBarFrame, &Init::playerHealthBar.dstRect);
    temp.w = Init::playerHealthBar.healthBarR;
    SDL_RenderCopy(game::Renderer, Init::playerHealthBar.healthbarTex, &Init::playerHealthBar.healthBar, &temp);
    temp.w = Init::playerHealthBar.energyBarR;
    SDL_RenderCopy(game::Renderer, Init::playerHealthBar.healthbarTex, &Init::playerHealthBar.energyBar, &temp);
}
void Render::renderText(int posX, int posY, std::string text, float charSize, int R, int G, int B)
{
    SDL_Rect srcRect;
    srcRect.w = CHAR_WIDTH;
    srcRect.h = CHAR_HEIGHT;
    srcRect.x = srcRect.y = 0;
    SDL_Rect dstRect;
    dstRect.x = posX;
    dstRect.y = posY;
    dstRect.w = CHAR_WIDTH * charSize;
    dstRect.h = CHAR_HEIGHT * charSize;
    SDL_SetTextureColorMod(wordChart, R, G, B);
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] >= ' ' && text[i] <= 'Z')
        {
            srcRect.x = (text[i] - ' ') * CHAR_WIDTH;
            SDL_RenderCopy(game::Renderer, wordChart, &srcRect, &dstRect);
            dstRect.x += CHAR_WIDTH * charSize;
        }
    }
}


void Render::renderGUI()
{
    std::string CD;
    SDL_Rect temp;
    temp.h = temp.w = 64;
    temp.x = SCREEN_WIDTH - 128;
    temp.y = SCREEN_HEIGHT - 70;

    SDL_RenderCopy(game::Renderer, Init::healSkill, NULL, &temp);
    CD = std::to_string(int(Player::heal_Cooldown / FPS));
    renderText(temp.x + 40, temp.y + 46, std::string(2 - CD.length(), '0').append(CD), 0.5, 255, 255, 255);

    CD = std::to_string(int(Player::drone_Cooldown / FPS));
    temp.x += 64;
    SDL_RenderCopy(game::Renderer, Init::droneSkill, NULL, &temp);
    renderText(temp.x + 40, temp.y + 46, std::string(2 - CD.length(), '0').append(CD), 0.5, 0, 0, 255);

    CD = std::to_string(game::score);
    Render::renderText(SCREEN_WIDTH - 200, 0, "SCORE:" + std::string(5 - CD.length(), '0').append(CD), 1, 0, 155, 155);
}
void Render::renderMenu()
{
    if (!game::isPlaying)
    {
        if (game::inMainMenu)
        {
            SDL_RenderCopy(game::Renderer, Init::menuTex, NULL, NULL);
            SDL_RenderCopy(game::Renderer, Init::button[PLAY_BUTTON].tex, NULL, &Init::button[PLAY_BUTTON].dstRect);
        }
        else
        {
            SDL_RenderCopy(game::Renderer, Init::gameOverTex, NULL, NULL);
            SDL_RenderCopy(game::Renderer, Init::button[MENU_BUTTON].tex, NULL, &Init::button[MENU_BUTTON].dstRect);
            SDL_RenderCopy(game::Renderer, Init::button[RETRY_BUTTON].tex, NULL, &Init::button[RETRY_BUTTON].dstRect);
            Render::renderText(0, 0, "SCORE: " + std::to_string(game::score), 1, 0, 155, 155);
        }
    }

}
