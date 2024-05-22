#include "home.h"
#include "textTexture.h"
#define android_build
#ifndef android_build
#include <SDL2/SDL_ttf.h>
#else
#include "SDL_ttf.h"
#endif
#include "util.h"
#include "draw.h"
#include <string.h>
void Home_show(SDL_Renderer *renderer,TextTexture *txtText, TTF_Font *font, Home *home)
{
    SDL_SetRenderDrawColor(renderer, home->color.r, home->color.g, home->color.b, home->color.a);
    SDL_RenderDrawRect(renderer, &home->rect);
     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &home->hpRect);
        SDL_SetRenderDrawColor(renderer, home->hpDangerColor.r, home->hpDangerColor.g, home->hpDangerColor.b, home->hpDangerColor.a);
        SDL_Rect fillRect2 = {home->hpRect.x + 1, home->hpRect.y + 1, home->hpRect.w - 2, home->hpRect.h - 2};
        SDL_RenderFillRect(renderer, &fillRect2);
        SDL_SetRenderDrawColor(renderer, home->hpHealthColor.r, home->hpHealthColor.g, home->hpHealthColor.b, home->hpHealthColor.a);
        SDL_Rect fillRect1 = {home->hpRect.x + 1, home->hpRect.y + 1, (home->hpRect.w * home->hp / home->hpmax - 2), home->hpRect.h - 2};
        SDL_RenderFillRect(renderer, &fillRect1);

    txtText->drawText(renderer, txtText, font, home->name, home->rect.x, home->rect.y, 1, txtText->defaultColor);

}
void Home_init(Home *self, const char *name,SDL_Rect rect, SDL_Color color,SDL_bool enabled)
{
    
    size_t len = strlen(name);
    self->name = malloc(len +1);
    memcpy(self->name, name, len);
    self->name[len] = '\0';
    self->rect = rect;
        self->hpRect = (SDL_Rect){rect.x - 5, rect.y - 8, 10 * 2, 4};
        self->hp = 1000;
        self->hpmax = 1000;
        self->hpHealthColor = (SDL_Color){0, 255, 0, 255};
        self->hpDangerColor = (SDL_Color){255, 0, 0, 255};
        self->attack = 1;
        self->attackRange = 30;
        self->attackRangeColor = (SDL_Color){255, 255, 255, 150};
        self->attackRangeShow = 0;
        self->attackColor = (SDL_Color){255, 0, 0, 255};
        self->attack_angle = 0;

    self->color = color;
    self->enabled = enabled;
}

CTOR(Home)
FUNCTION_SETTING(show,Home_show);
FUNCTION_SETTING(init,Home_init);
// FUNCTION_SETTING(bornNewHero, Home_bornNewHero);
// FUNCTION_SETTING(heroAddHp, Home_heroAddHp);
END_CTOR