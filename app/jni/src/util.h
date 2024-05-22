#ifndef util_h
#define util_h
#define android_build
#ifndef android_build
#include <SDL2/SDL.h>

#else
#include "SDL.h"

#endif
#define PI 3.14159
#define LENGTH(x, y) SDL_sqrt((x) * (x) + (y) * (y))
#define WINDOW_W 2000
#define WINDOW_H 1080
#define MARGIN 50

float length(float x, float y);
int pointInRect(float x, float y, SDL_Rect *rect);
int pointInCircle(float x, float y, float x2, float y2, float r2);
#endif