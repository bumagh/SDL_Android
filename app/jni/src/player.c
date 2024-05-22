#include <stdlib.h>
#include "skillButton.h"

#define android_build
#ifndef android_build
#include <SDL2/SDL_ttf.h>
#else
#include "SDL_ttf.h"
#endif
#include "player.h"
#include "draw.h"
#include "util.h"
extern Hero *pHeros[100];
extern int pHeroCounts;
void Hero_show(SDL_Renderer *renderer, Hero *hero,TextTexture *textTexture, TTF_Font *font, int type)
{
       if (hero->rect.x < 0)
            hero->rect.x = 5;
        if (hero->rect.y < 0)
            hero->rect.y = 5;
        if (hero->rect.y > WINDOW_H - 2 * MARGIN)
            hero->rect.y = WINDOW_H - 2 * MARGIN - 5;
        if (hero->rect.x > WINDOW_W - 5)
            hero->rect.x = WINDOW_W - 5;
    if (type == 1) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   else
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_RenderFillRect(renderer, &hero->rect);

        // draw hp
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &hero->hpRect);
        SDL_SetRenderDrawColor(renderer, hero->hpDangerColor.r, hero->hpDangerColor.g, hero->hpDangerColor.b, hero->hpDangerColor.a);
        SDL_Rect fillRect2 = {hero->hpRect.x + 1, hero->hpRect.y + 1, hero->hpRect.w - 2, hero->hpRect.h - 2};
        SDL_RenderFillRect(renderer, &fillRect2);
        SDL_SetRenderDrawColor(renderer, hero->hpHealthColor.r, hero->hpHealthColor.g, hero->hpHealthColor.b, hero->hpHealthColor.a);
        SDL_Rect fillRect1 = {hero->hpRect.x + 1, hero->hpRect.y + 1, (hero->hpRect.w * hero->hp / hero->hpmax - 2), hero->hpRect.h - 2};
        SDL_RenderFillRect(renderer, &fillRect1);
        //draw hp number
         char buf[20];
        sprintf(buf, "%d", hero->hp);
     textTexture->drawText(renderer, textTexture,font,buf, hero->rect.x + hero->rect.w / 2 - 2 * 15 / 2, hero->rect.y + hero->rect.h / 2 - 15 / 2, 1, textTexture->defaultColor);

        // draw attack range
        if (hero->attackRangeShow)
        {
            SDL_SetRenderDrawColor(renderer, hero->attackRangeColor.r, hero->attackRangeColor.g, hero->attackRangeColor.b, hero->attackRangeColor.a);
            drawCircle(renderer, hero->rect.x + hero->rect.w / 2, hero->rect.y + hero->rect.h / 2, hero->attackRange);
            // draw attack anim
            drawAttackLine(renderer, hero->rect.x + hero->rect.w / 2, hero->rect.y + hero->rect.h / 2, hero->attackRange, hero->l_angle);
        }
    
}
void Hero_move(Hero *player, int speed)
{
    player->rect.x += 5 * player->speed * SDL_cos(player->l_angle / 180 * PI);
    player->rect.y += 5 * player->speed * SDL_sin(player->l_angle / 180 * PI);
    player->hpRect.x = player->rect.x - player->rect.w / 2;
    player->hpRect.y = player->rect.y - player->hpRect.h * 2;
}
void Hero_action(Hero *hero, SDL_Rect dest)
{
   if ((dest.x - hero->rect.x) != 0)
        hero->l_angle = SDL_atan((dest.y - hero->rect.y) / (dest.x - hero->rect.x)) * 180 / PI + 180 * (dest.x < hero->rect.x);
}

void Hero_moveByDir(Hero *player, int dir, int speed)
{
    if (dir == 'w')
    {
        player->rect.y = player->rect.y - 5 * speed;
        player->l_angle = 270;
        player->hpRect.x = player->rect.x - player->rect.w / 2;
        player->hpRect.y = player->rect.y - player->hpRect.h * 2;
    }
    else if (dir == 'd')
    {
        player->rect.x = player->rect.x + 5 * speed;
        player->l_angle = 0;
        player->hpRect.x = player->rect.x - player->rect.w / 2;
        player->hpRect.y = player->rect.y - player->hpRect.h * 2;
    }
    else if (dir == 's')
    {
        player->rect.y = player->rect.y + 5 * speed;
        player->l_angle = 90;
        player->hpRect.x = player->rect.x - player->rect.w / 2;
        player->hpRect.y = player->rect.y - player->hpRect.h * 2;
    }
    else if (dir == 'a')
    {
        player->rect.x = player->rect.x - 5 * speed;
        player->hpRect.x = player->rect.x - player->rect.w / 2;
        player->hpRect.y = player->rect.y - player->hpRect.h * 2;
        player->l_angle = 180;
    }
}

void Hero_init(Hero *self, const char *name, SDL_Rect rect, int type)
{
    size_t len = strlen(name);
    self->name = malloc(len +1);
    memcpy(self->name, name, len);
    self->name[len] = '\0';
    self->rect = rect;
    self->type = type;
    self->state = 0;//0stop 1move 2 attack 3 return home 5 curing 6 die
    self->exp = 0;
    self->level = 1;
    self->coin = 300;
   
       self->l_angle = 0;
        self->hpRect = (SDL_Rect){rect.x - 5, rect.y - 8, 10 * 2, 4};
        self->hp = 100;
        self->hpmax = 100;
        self->hpHealthColor = (SDL_Color){0, 255, 0, 255};
        self->hpDangerColor = (SDL_Color){255, 0, 0, 255};
        self->attack = 1;
        self->attackColor = (SDL_Color){255, 0, 0, 255};
        self->attack_angle = 0;
        self->attackRangeShow = 0;

    if (type == 1)
    {
      self->speed = 1;
        self->attackRange = 30;
        self->attackRangeColor = (SDL_Color){255, 255, 255, 150};
    }
    else
    {
        self->speed = rand() % 3 + 1;
        self->attackRange = 10+rand() % 40 + 10;
        self->attackRangeColor = (SDL_Color){255, rand() % 255, 0, 255};
    }
  pHeros[pHeroCounts] = self;
    pHeroCounts+=1;
}
void newHero(const char *name, SDL_Rect rect, int type)
{
    // Hero  hero = new (Hero)(name, rect, type);
    //  pHeros[pHeroCounts] = hero;
    // pHeroCounts+=1;
    // return hero;
}
void freeHero(Hero self)
{
   
    
}

CTOR(Hero)
FUNCTION_SETTING(show,Hero_show);
FUNCTION_SETTING(init,Hero_init);
FUNCTION_SETTING(move,Hero_move);
FUNCTION_SETTING(moveByDir,Hero_moveByDir);
FUNCTION_SETTING(action,Hero_action);

// FUNCTION_SETTING(bornNewHero, Home_bornNewHero);
// FUNCTION_SETTING(heroAddHp, Home_heroAddHp);
END_CTOR