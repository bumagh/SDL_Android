#include "skillButton.h"
#define android_build
#ifndef android_build
#include <SDL2/SDL_ttf.h>

#else
#include "SDL_ttf.h"
#endif
#include "textTexture.h"
#include "util.h"
#include "draw.h"
#include <string.h>

extern SkillButton *pSkillButtons[100];
extern int pSkillButtonCounts;
void SkillButton_show(SDL_Renderer *renderer, SkillButton *skillButton,TextTexture *textTexture, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer,skillButton->color.r, skillButton->color.g, skillButton->color.b, skillButton->color.a);
    fillCircle(renderer, skillButton->rect.x + skillButton->rect.w / 2, skillButton->rect.y + skillButton->rect.h / 2, skillButton->rect.w / 2);

    SDL_SetRenderDrawColor(renderer, skillButton->maskColor.r, skillButton->maskColor.g, skillButton->maskColor.b, skillButton->maskColor.a);
    if (skillButton->enabled == SDL_FALSE)
    {
        if (skillButton->curTime > 0)
            skillButton->curTime--;
        else
        {
            skillButton->curTime = skillButton->coldTime;
            skillButton->enabled = SDL_TRUE;
        }
        fillCircleByAngle(renderer, skillButton->rect.x + skillButton->rect.w / 2, skillButton->rect.y + skillButton->rect.h / 2, skillButton->rect.w / 2, 360 * ((skillButton->curTime / (skillButton->coldTime * 1.0))));
    }

     textTexture->drawText(renderer, textTexture,font, skillButton->name, skillButton->rect.x + skillButton->rect.w / 2 - 2 * 15 / 2, skillButton->rect.y + skillButton->rect.h / 2 - 15 / 2, 1, textTexture->defaultColor);

}
void SkillButton_setClickEvent(SkillButton *skillButton, void (*func)())
{
    skillButton->clickEvent = func;
}
void SkillButton_clickEvent()
{
}
void SkillButton_initByWH(int w, int h, SkillButton btn)
{
  
}
void SkillButton_init(SkillButton *self, const char *name, SDL_Rect rect, int curTime, int coldTime, int bindKey)
{
    size_t len = strlen(name);
    self->name = malloc(len + 1);
    memcpy(self->name, name, len);
    self->name[len] = '\0';
    self->rect = rect;
    SDL_Rect dxyrect = {self->rect.x-WINDOW_W, self->rect.y - WINDOW_H,0,0};
    self->dxyRect = dxyrect;
    self->curTime = curTime;
    self->coldTime = coldTime;
    self->enabled = SDL_TRUE;
    self->bindKey = bindKey;
    self->color = (SDL_Color){0, 0, 255, 255};
    self->maskColor = (SDL_Color){155, 100, 155, 255};
  pSkillButtons[pSkillButtonCounts] = self;
    pSkillButtonCounts+=1;
}

void newSkillButton(const char *name,SDL_Rect rect, int curTime, int coldTime, int bindKey)
{

}

void freeSkillButton(SkillButton self)
{
}

CTOR(SkillButton)
FUNCTION_SETTING(show,SkillButton_show);
FUNCTION_SETTING(init,SkillButton_init);
FUNCTION_SETTING(setClickEvent,SkillButton_setClickEvent);
FUNCTION_SETTING(clickEvent,SkillButton_clickEvent);
FUNCTION_SETTING(initByWH,SkillButton_initByWH);
END_CTOR