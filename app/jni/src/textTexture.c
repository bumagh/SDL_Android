#include "textTexture.h"
#include "util.h"
#include "draw.h"
#include <string.h>
void TextTexture_show(SDL_Renderer *renderer, TextTexture textTexture)
{
   
}
void TextTexture_drawText(SDL_Renderer *renderer, TextTexture *textTexture,TTF_Font *font,char *text, int x, int y, float fontscale, SDL_Color textColor)
{
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, text, textColor);
    SDL_DestroyTexture(textTexture->txtText);
    textTexture->txtText = SDL_CreateTextureFromSurface(renderer, textSurface);
    textTexture->txtDstRect = (SDL_Rect){0, 0, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);
     textTexture->txtDstRect.x = x;
    textTexture->txtDstRect.y = y;
    SDL_RenderSetScale(renderer, fontscale, fontscale);
    SDL_RenderCopy(renderer, textTexture->txtText, NULL, &textTexture->txtDstRect);
}

void TextTexture_setClickEvent(TextTexture *textTexture, void (*func)())
{
}
void TextTexture_clickEvent()
{
}

void TextTexture_init(TextTexture *self, const char *name,const char *text, SDL_bool enabled)
{
    SDL_Color defaultColor = {255, 0, 0, 255};
    size_t len = strlen(name);
    self->name = malloc(len + 1);
    memcpy(self->name, name, len);
    self->name[len] = '\0';
    size_t len2 = strlen(text);
    self->text = malloc(len + 1);
    memcpy(self->text, text, len2);
    self->text[len2] = '\0';
    self->enabled = enabled;
    self->color =defaultColor;
    self->defaultColor = defaultColor;
}

void newTextTexture(const char *name,const char *text, SDL_bool enabled)
{
}

void freeTextTexture(TextTexture self)
{
}
CTOR(TextTexture)
FUNCTION_SETTING(show,TextTexture_show);
FUNCTION_SETTING(init,TextTexture_init);
FUNCTION_SETTING(drawText,TextTexture_drawText);
FUNCTION_SETTING(clickEvent,TextTexture_clickEvent);
FUNCTION_SETTING(setClickEvent,TextTexture_setClickEvent);
END_CTOR