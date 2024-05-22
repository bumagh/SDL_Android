#define android_build
#ifndef android_build
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#endif
#include "lw_oopc.h"
#ifndef textTexture_h
#define textTexture_h

CLASS(TextTexture){
          char *name;
          char *text;
         SDL_Texture *txtText;
         SDL_Rect txtDstRect;
          SDL_bool enabled;
          SDL_Color color;
          SDL_Color defaultColor;
          void (*init)(TextTexture *self, const char *name, const char *text, SDL_bool enabled);
          void (*show)(SDL_Renderer *renderer, TextTexture *textTexture);
          void (*drawText)(SDL_Renderer *renderer, TextTexture *textTexture,TTF_Font *font,char *text, int x, int y, float fontscale, SDL_Color textcolor);
        //   void (*setTextTexture)(SDL_Renderer *renderer, TextTexture textTexture, TTF_Font *font, char *text, SDL_Color textColor);
          void (*clickEvent)();
          void (*setClickEvent)(TextTexture *textTexture, void(*event));
};

void newTextTexture(const char *name, const char *text , SDL_bool enabled);
void freeTextTexture(TextTexture self);

#endif