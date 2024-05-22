#include "touchButton.h"
#include "util.h"
#include "draw.h"
#include <string.h>
void TouchButton_show(SDL_Renderer *renderer, TouchButton *touchButton)
{
    float i;

    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    for (i = 0; i < 360; i += 2)
        SDL_RenderDrawLine(renderer,
                           (touchButton->mover + 2) * SDL_cos(i / 180 * PI) + touchButton->movex,
                           (touchButton->mover + 2) * SDL_sin(i / 180 * PI) + touchButton->movey,
                           (touchButton->mover + 1) * SDL_cos(i / 180 * PI) + touchButton->movex,
                           (touchButton->mover + 1) * SDL_sin(i / 180 * PI) + touchButton->movey);
    for (i = 0; i < 360; i += 2)
        SDL_RenderDrawPoint(renderer,
                            touchButton->mover * SDL_cos(i / 180 * PI) + touchButton->movex,
                            touchButton->mover * SDL_sin(i / 180 * PI) + touchButton->movey);
    for (i = 0; i < 360; i++)
        SDL_RenderDrawPoint(renderer,
                            touchButton->outr * SDL_cos(i / 180 * PI) + touchButton->outx,
                            touchButton->outr * SDL_sin(i / 180 * PI) + touchButton->outy);
}
void TouchButton_initByWH(int w, int h, TouchButton *touchButton)
{
    touchButton->movex = w / 7;
    touchButton->movey = h * 3.8 / 8.0;
    touchButton->mover = w / 16;
    touchButton->outx = w / 7;
    touchButton->outy = h * 3.8 / 8.0;
    touchButton->outr = w / 10;
}

void TouchButton_mouseButtonDown(SDL_Event e, TouchButton *touchButton)
{
//    if (LENGTH(touchButton->outx - e.button.x, touchButton->outy - e.button.y) < touchButton->outr)
//    {
//        touchButton->touchStart = SDL_TRUE;
//    }
//    else
//    {
//        touchButton->touchStart = SDL_FALSE;
//    }
//    if (LENGTH(touchButton->outx - touchButton->movex, touchButton->outy - touchButton->movey) > touchButton->outr)
//    {
//        touchButton->movex = touchButton->outx;
//        touchButton->movey = touchButton->outy;
//    }
//    else
//    {
//        if (LENGTH(touchButton->outx - e.button.x, touchButton->outy - e.button.y) < touchButton->outr)
//        {
//            touchButton->movex = e.button.x;
//            touchButton->movey = e.button.y;
//        }
//    }

//
//    if (LENGTH(touchButton->outx - e.tfinger.x*w, touchButton->outy - e.tfinger.y*h) < touchButton->outr)
//    {
//        touchButton->touchStart = SDL_TRUE;
//    }
//    else
//    {
//        touchButton->touchStart = SDL_FALSE;
//    }
//    if (LENGTH(touchButton->outx - touchButton->movex, touchButton->outy - touchButton->movey) > touchButton->outr)
//    {
//        touchButton->movex = touchButton->outx;
//        touchButton->movey = touchButton->outy;
//    }
//    else
//    {
//        if (LENGTH(touchButton->outx - e.button.x, touchButton->outy - e.button.y) < touchButton->outr)
//        {
//            touchButton->movex = e.button.x;
//            touchButton->movey = e.button.y;
//        }
//    }
}
void TouchButton_mouseButtonUp(SDL_Event e, TouchButton *touchButton)
{
    touchButton->touchStart = SDL_FALSE;
    touchButton->movex = touchButton->outx;
    touchButton->movey = touchButton->outy;
}
void TouchButton_mouseMotion(SDL_Event e, TouchButton *touchButton)
{
    // touchButton->mouseMotion(e,touchButton,player);
    if (touchButton->touchStart == SDL_TRUE)
    {
        if (LENGTH(touchButton->outx - e.button.x, touchButton->outy - e.button.y) < touchButton->outr)
        {
            touchButton->movex = e.button.x;
            touchButton->movey = e.button.y;
        }
    }
    if (LENGTH(touchButton->outx - touchButton->movex,
               touchButton->outy - touchButton->movey) > touchButton->outr)
    {
        touchButton->movex = touchButton->outx;
        touchButton->movey = touchButton->outy;
    }
}
void TouchButton_keyEvent(SDL_Event e, TouchButton *touchButton)
{
}

void TouchButton_init(TouchButton *self, const char *name)
{
    size_t len = strlen(name);
    self->name = malloc(len + 1);
    memcpy(self->name, name, len);
    self->name[len] = '\0';
    self->movex = 1;
    self->movey = 1;
    self->mover = 1;
    self->outx = 2;
    self->outy = 2;
    self->outr = 2;
    self->touchStart = SDL_FALSE;
}

void newTouchButton(const char *name)
{
}

void freeTouchButton(TouchButton self)
{
}

CTOR(TouchButton)
FUNCTION_SETTING(show,TouchButton_show);
FUNCTION_SETTING(init,TouchButton_init);
FUNCTION_SETTING(initByWH,TouchButton_initByWH);
FUNCTION_SETTING(mouseButtonDown,TouchButton_mouseButtonDown);
FUNCTION_SETTING(mouseButtonUp,TouchButton_mouseButtonUp);
FUNCTION_SETTING(keyEvent,TouchButton_keyEvent);
FUNCTION_SETTING(mouseMotion,TouchButton_mouseMotion);
END_CTOR