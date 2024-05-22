#define android_build
#ifndef android_build
#include <SDL2/SDL.h>

#else
#include "SDL.h"

#endif
#include "lw_oopc.h"
#ifndef uiText_h
#define uiText_h
CLASS(UiText){
          char *name;
          char *text;
          SDL_Rect rect;
          SDL_bool enabled;
          SDL_KeyCode bindKey;
          SDL_Color color;
          SDL_Color textColor;
          void (*init)(UiText *self,const char *name, const char *text, SDL_Rect rect, SDL_bool enabled, SDL_KeyCode bindKey, SDL_Color color, SDL_Color textColor);
          void (*full_screen_toggle)(UiText *uiText, SDL_Window *window, SDL_bool full_screen_q);
          void (*show)(SDL_Renderer *renderer, UiText *uiText);
          void (*clickEvent)();
          void (*setClickEvent)(UiText *uiText, void(*event));
};
void newUiText(const char *name, const char *text, SDL_Rect rect, SDL_bool enabled, SDL_KeyCode bindKey, SDL_Color color, SDL_Color textColor);
void freeUiText(UiText self);

#endif