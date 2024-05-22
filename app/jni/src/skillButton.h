#define android_build
#ifndef android_build
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#endif
#include "textTexture.h"
#include"lw_oopc.h"
#ifndef skillButton_h
#define skillButton_h

CLASS(SkillButton){
          char *name;
          SDL_Rect rect;
          int curTime;
          int coldTime;
          SDL_bool enabled;
          int bindKey;
          SDL_Color color;
          SDL_Color maskColor;
          SDL_Rect dxyRect;
          void (*init)(SkillButton *skillbutton, const char *name,SDL_Rect rect, int curTime, int coldTime, int bindKey);
          void (*show)(SDL_Renderer *renderer, SkillButton *skillButton,TextTexture *textTexture, TTF_Font *font);
          void (*clickEvent)();
          void (*initByWH)();
          void (*setClickEvent)(SkillButton *skillButton,void(*event));
};

void newSkillButton(const char *name,SDL_Rect rect, int curTime, int coldTime, int bindKey);
void freeSkillButton(SkillButton self);

#endif