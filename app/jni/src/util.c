#define android_build
#ifndef android_build
#include <SDL2/SDL.h>

#else
#include "SDL.h"

#endif

int pointInRect(float x, float y, SDL_Rect *rect)
{
    if (x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h)
        return 1;
    return 0;
}
float length(float x, float y){
    return SDL_sqrt((x) * (x) + (y) * (y));
}
int pointInCircle(float x, float y, float x2, float y2, float r2){
    if((length(x-x2, y-y2)) < r2)
    return 1;
    return 0;
}