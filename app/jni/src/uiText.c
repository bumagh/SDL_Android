#include "uiText.h"
#include "util.h"
#include "draw.h"
#include "skillButton.h"
#include <string.h>
extern SkillButton *pSkillButtons[100];
extern int pSkillButtonCounts;
void UiText_show(SDL_Renderer *renderer, UiText *uiText)
{
    SDL_SetRenderDrawColor(renderer, uiText->color.r, uiText->color.g, uiText->color.b, uiText->color.a);
    SDL_RenderFillRect(renderer, &uiText->rect);
}
void UiText_setClickEvent(UiText *uiText, void (*func))
{
}
void UiText_clickEvent()
{
}
void UiText_full_screen_toggle(UiText *uiText, SDL_Window *window, SDL_bool full_screen_q)
{
      int ret_val = 0;
      int w, h;
    // 关闭全屏
    if (full_screen_q)
    {
        ret_val = SDL_SetWindowFullscreen(window, 0);
        SDL_GetWindowSize(window, &w, &h);
    }
    else
    {
        // 开启全屏
        ret_val = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_GetWindowSize(window, &w, &h);
    }
    for (int i = 0; i < pSkillButtonCounts; i++){
        pSkillButtons[i]->rect.x = w + pSkillButtons[i]->dxyRect.x;
        pSkillButtons[i]->rect.y = h + pSkillButtons[i]->dxyRect.y;
            }

    if (ret_val < 0)
    {
        printf("Full screen toggle error: %s\n", SDL_GetError());
    }
    else
    {
       // printf("Full screen toggle successful\n");

    }
}
void UiText_init(UiText *self, const char *name,const char *text, SDL_Rect rect, SDL_bool enabled, SDL_KeyCode bindKey, SDL_Color color, SDL_Color textColor)
{
    size_t len = strlen(name);
    self->name =malloc(len + 1);
    memcpy(self->name, name, len);
    self->name[len] = '\0';
    size_t len2 = strlen(text);
    self->text = malloc(len + 1);
    memcpy(self->text, text, len2);
    self->text[len2] = '\0';
    self->rect = rect;
    self->enabled = enabled;
    self->bindKey = bindKey;
    self->color =color;
    self->textColor = textColor;
}

void newUiText(const char *name,const char *text, SDL_Rect rect, SDL_bool enabled, SDL_KeyCode bindKey, SDL_Color color, SDL_Color textColor)
{
}

void freeUiText(UiText self)
{
}

CTOR(UiText)
FUNCTION_SETTING(show,UiText_show);
FUNCTION_SETTING(init,UiText_init);
FUNCTION_SETTING(setClickEvent,UiText_setClickEvent);
FUNCTION_SETTING(clickEvent,UiText_clickEvent);
FUNCTION_SETTING(full_screen_toggle,UiText_full_screen_toggle);

END_CTOR