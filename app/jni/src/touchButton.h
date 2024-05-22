#define android_build
#ifndef android_build
#include <SDL2/SDL.h>

#else
#include "SDL.h"

#endif
#include "lw_oopc.h"
#ifndef touchButton_h
#define touchButton_h

CLASS(TouchButton){
 
          char *name;
          float movex;
          float movey;
          float mover;
          float outx;
          float outy;
          float outr;
          SDL_bool touchStart;
          void (*init)(TouchButton *touchButton,const char *name);
          void (*show)(SDL_Renderer *renderer, TouchButton *touchButton);
          void (*initByWH)(int w, int h, TouchButton *touchButton);
          void (*mouseButtonDown)(SDL_Event e, TouchButton *touchButton);
          void (*mouseButtonUp)(SDL_Event e, TouchButton *touchButton);
          void (*mouseMotion)(SDL_Event e, TouchButton *touchButton);
          void (*keyEvent)(SDL_Event e, TouchButton *touchButton);
};

void newTouchButton(const char *name);
void freeTouchButton(TouchButton self);

#endif