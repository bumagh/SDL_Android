#include "skillButton.h"

#define android_build
#ifndef android_build
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#endif
#include"lw_oopc.h"

#ifndef player_h
#define player_h

CLASS(Hero){
        char *name;
        SDL_Rect rect;
        float l_angle;
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
        int type;
        int state;//0stop 1move 2 attack 3 return home 5 curing 6 die
        int exp;
        int level;
        int coin;
        int speed;
        void (*init)(Hero *self,const char *name, SDL_Rect rect, int type);
        void (*action)(Hero *hero, SDL_Rect dest);
        void (*show)(SDL_Renderer *renderer, Hero *hero,TextTexture *textTexture, TTF_Font *fon, int type);
        void (*move)(Hero *hero, int speed);
        void (*moveByDir)(Hero *hero,int dir, int speed);
};

void newHero(const char *name, SDL_Rect rect, int type);
void freeHero(Hero self);

#endif