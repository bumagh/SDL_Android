#define android_build
#ifndef android_build
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#endif
#include<stdio.h>
#include"lw_oopc.h"
#include "textTexture.h"
#include "player.h"
#ifndef home_h
#define home_h

CLASS(Home){
        char *name;
          SDL_Rect rect;
          SDL_Rect hpRect;
          int hp;
          int hpmax;
          SDL_Color hpHealthColor;
          SDL_Color hpDangerColor;
          int attack;
          float attackRange;
          SDL_Color attackRangeColor;
          int attackRangeShow;
          SDL_Color attackColor;
          float attack_angle;
          SDL_Color color;
          SDL_bool enabled;
          void (*init)(Home *self, const char *name,SDL_Rect rect, SDL_Color color,SDL_bool enabled);
          void (*show)(SDL_Renderer *renderer,TextTexture *textTexture, TTF_Font *font, Home *home);
          void (*bornNewHero)(Home home,SDL_Renderer *renderer,int type);
          void (*heroAddHp)(Home home, Hero hero);
};


Home newHome(const char *name, SDL_Rect rect, SDL_Color color, SDL_bool enabled);
void freeHome(Home self);

#endif