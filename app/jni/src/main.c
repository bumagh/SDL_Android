#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define android_build
#ifndef android_build
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#else
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "player.h"
#include "skillButton.h"
#include "touchButton.h"
#include "home.h"
#include "uiText.h"
#include "textTexture.h"
#include "util.h"
#include "draw.h"
#include "global.h"
int w, h; // 屏幕大小
#define SDL_DELAY 12
#define FRAMERATE 60

int success = 1;
SDL_Event e;
int gQuit = 0;
float mouseX = 0;
float mouseY = 0;
int fingerSupport = 1;

int numFrames = 0;
int startTime = 0;
SDL_Color fpsColor = {255, 0, 0, 255};
UiText *menuFullScreen;

Hero *player;
Hero *enemy;

TouchButton *touchButton;
Home* playerHome;
Home* enemyHome;
SkillButton *flashButton;
SkillButton *returnHomeButton;
SkillButton *attackButton;

SkillButton *cureButton;
SkillButton *upButton;

SkillButton *enemyAttackButton;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_bool full_screen = SDL_FALSE;
TTF_Font *font;
TextTexture *txtText1;
SDL_Color textMenuColor = {0, 0, 0, 255};

Mix_Chunk *gameMusic;
Mix_Chunk *attackMusic;
Mix_Chunk *damageMusic;
Mix_Chunk *flashMusic;
Mix_Chunk *returnHomeMusic;
int gMusicCondition = 1;

int init()
{
    srand(time(NULL));
   // Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
   // Mix_AllocateChannels(16);
    if (TTF_Init() == -1)
    {
        SDL_Log("TTF unable to initialize! Error: %s\n", TTF_GetError());
        success = 0;
    }
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = 0;
    }
    else
    {
        window = SDL_CreateWindow("SDL小小荣耀", 0, 0, 0, 0, SDL_WINDOWEVENT_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = 0;
        }
        else
        {
            // Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = 0;
            }
        }
    }
    return success;
}
void switchMusic()
{
    gMusicCondition = !gMusicCondition;
}

Mix_Chunk *loadWAV(const char *filepath) {
  SDL_RWops *fileData = SDL_RWFromFile(filepath, "rb");
  if(!fileData) {
    printf("SDL_RWFromFile: %s\n", SDL_GetError());
    return NULL;
  }
  
  Mix_Chunk *chunk = Mix_LoadWAV_RW(fileData, 1);
  if(!chunk){
    printf("Mix_LoadWAV_RW: %s\n", Mix_GetError());
    SDL_FreeRW(fileData);
    return NULL;
  }
  
  return chunk;
}
void playMusic()
{
    if (gMusicCondition)
    {
        if (Mix_Paused(1))
        {
            Mix_Resume(1);
        }
        else
        {
            Mix_PlayChannel(1, gameMusic, -1);
        }
    }
}

void full_screen_toggle(SDL_Window *window, SDL_bool full_screen_q)
{

    menuFullScreen->full_screen_toggle(menuFullScreen, window, full_screen_q);
}

void returnHomeButtonEvent()
{

    player->rect.x = playerHome->rect.x + playerHome->rect.w / 2;
    player->rect.y = playerHome->rect.y + playerHome->rect.h / 2;
    player->hpRect.x = player->rect.x - player->rect.w / 2;
    player->hpRect.y = player->rect.y - player->hpRect.h * 2;
}

void waitSecondsTodo(void *argc)
{
    SDL_Delay(((int)argc) * 1000);
    returnHomeButtonEvent();
}
void drawFps()
{
    int elapsedMS = SDL_GetTicks() - startTime; // Time since start of loop
    ++numFrames;
    if (elapsedMS)
    {                                                // Skip this the first frame
        double elapsedSeconds = elapsedMS / 1000.0;  // Convert to seconds
        int fps = (int)(numFrames / elapsedSeconds); // FPS is Frames / Seconds
        char buf[20];
        sprintf(buf, "FPS:%d", fps);
        // printf("buf = %s\n", buf);
        txtText1->drawText(renderer, txtText1, font, buf, 200, 10, 1.0f, txtText1->defaultColor);
    }
}
void drawMenuFullScreen()
{
    menuFullScreen->show(renderer, menuFullScreen);
    if (full_screen)
    {
        txtText1->drawText(renderer, txtText1, font, "关全屏", menuFullScreen->rect.x + menuFullScreen->rect.w / 2 - 7 - 3 * 15 / 2, menuFullScreen->rect.y + menuFullScreen->rect.h / 2 - 7, 1, textMenuColor);
    }
    else
    {
        txtText1->drawText(renderer, txtText1, font, "开全屏", menuFullScreen->rect.x + menuFullScreen->rect.w / 2 - 7 - 3 * 15 / 2, menuFullScreen->rect.y + menuFullScreen->rect.h / 2 - 7, 1, textMenuColor);
    }
}
void attackEnemys(Hero *enemy)
{
    if (pointInCircle(enemy->rect.x, enemy->rect.y, player->rect.x, player->rect.y, player->attackRange))
    {
        if (player->rect.x - enemy->rect.x != 0)
            player->attack_angle = SDL_atan((player->rect.y - enemy->rect.y) / (player->rect.x - enemy->rect.x)) * 180 / PI + 180 * (player->rect.x > enemy->rect.x);
        if (enemy->hp > 0)
        {
            enemy->hp -= player->attack;
            // printf("你攻击了敌方,敌方hp-2\n");
        }
        if (enemy->hp <= 0)
        {
            printf("你杀死了敌人");
            // born new enemy
            enemy->hp = 100;
            enemy->rect.x = enemyHome->rect.x;
            enemy->rect.y = enemyHome->rect.y;
            // add coin
            player->coin = player->coin + 50;
        }
    }
    else
    {
        player->attack_angle = player->l_angle;
    }
}
void cureButtonEvent()
{
    player->hp = player->hp + (player->hpmax - player->hp) / 3;
}

void attackButtonEvent()
{
    // Mix_PlayChannel(-1, attackMusic, 0);
    player->attackRangeShow = 1;

    // for (int i = 0; i < pHeroCounts; i++)
    // {
    //     // enemy
    //     if (((pHeros[i]))->type == 2)
    //         attackEnemys((pHeros[i]));
    // }
}
void enemyAttackEvent()
{
    if(player->hp >=2)
    player->hp -= 2;
    // Mix_PlayChannel(-1, damageMusic, 1);
    // printf("你受到敌方攻击,hp-2\n");
}
void touchButtonEvent()
{

    switch (e.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        touchButton->mouseButtonDown(e, touchButton);
        break;
    case SDL_MOUSEMOTION:
        touchButton->mouseMotion(e, touchButton);
        if (touchButton->touchStart == SDL_TRUE)
        {
            player->l_angle = SDL_atan((e.button.y - touchButton->outy) / (e.button.x - touchButton->outx)) * 180 / PI + 180 * (e.button.x < touchButton->outx);
            player->move(player, 1);
        }
        case SDL_FINGERDOWN:
//            touchButton->mouseButtonDown(e, touchButton);
            SDL_GetWindowSize(window, &w, &h);

            if (LENGTH(touchButton->outx - e.tfinger.x*w, touchButton->outy - e.tfinger.y*h) < touchButton->outr)
            {
                touchButton->touchStart = SDL_TRUE;
            }
            else
            {
                touchButton->touchStart = SDL_FALSE;
            }
            if (LENGTH(touchButton->outx - touchButton->movex, touchButton->outy - touchButton->movey) > touchButton->outr)
            {
                touchButton->movex = touchButton->outx;
                touchButton->movey = touchButton->outy;
            }
            else
            {
                if (LENGTH(touchButton->outx - e.tfinger.x*w, touchButton->outy - e.tfinger.y*h) < touchButton->outr)
                {
                    touchButton->movex = e.tfinger.x*w;
                    touchButton->movey = e.tfinger.y*h;
                }
            }
            break;
        case SDL_FINGERMOTION:
            SDL_GetWindowSize(window, &w, &h);
            if (touchButton->touchStart == SDL_TRUE)
            {
                if (LENGTH(touchButton->outx - e.tfinger.x*w, touchButton->outy - e.tfinger.y*h) < touchButton->outr)
                {
                    touchButton->movex = e.tfinger.x*w;
                    touchButton->movey = e.tfinger.y*h;
                }
            }
            if (LENGTH(touchButton->outx - touchButton->movex,
                       touchButton->outy - touchButton->movey) > touchButton->outr)
            {
                touchButton->movex = touchButton->outx;
                touchButton->movey = touchButton->outy;
            }
//            touchButton->mouseMotion(e, touchButton);
            if (touchButton->touchStart == SDL_TRUE)
            {

                player->l_angle = SDL_atan((e.tfinger.y*h - touchButton->outy) / (e.tfinger.x*w - touchButton->outx)) * 180 / PI + 180 * (e.tfinger.x*w < touchButton->outx);
                player->move(player, 1);
            }
            break;
        break;
    case SDL_MOUSEBUTTONUP:
        touchButton->mouseButtonUp(e, touchButton);

        break;
    }
}
void flashButtonEvent()
{
    player->move(player, 8);
   // Mix_PlayChannel(-1, flashMusic, 0);
}
void handleDown()
{
    float x = 0, y = 0;
    x = e.button.x;
    y = e.button.y;
    if (fingerSupport)
    {
        x = mouseX;
        y = mouseY;
    }
    if (pointInRect(x, y, &flashButton->rect))
    {

        if (flashButton->enabled == SDL_TRUE)
        {
            flashButtonEvent();
            flashButton->enabled = SDL_FALSE;
        }
    }

    if (pointInRect(x, y, &returnHomeButton->rect))
    {
        if (returnHomeButton->enabled == SDL_TRUE)
        {
            returnHomeButtonEvent();
            returnHomeButton->enabled = SDL_FALSE;
        }
    }
    if (pointInRect(x, y, &attackButton->rect))
    {
        if (attackButton->enabled == SDL_TRUE)
        {
            attackButtonEvent();
            attackButton->enabled = SDL_FALSE;
        }
    }
    if (pointInRect(x, y, &cureButton->rect))
    {
        if (cureButton->enabled == SDL_TRUE)
        {
            cureButtonEvent();
            cureButton->enabled = SDL_FALSE;
        }
    }
    if (pointInRect(x, y, &menuFullScreen->rect))
    {
        full_screen_toggle(window, full_screen);
        full_screen = !full_screen;
    }
}
void interpolateFinger()
{
    SDL_Finger finger;
    SDL_FingerID index = e.tfinger.fingerId;
    finger.id = index;
    finger.x = e.tfinger.x;
    finger.y = e.tfinger.y;
    SDL_GetWindowSize(window, &w, &h);
    mouseX = (float)(w)*e.tfinger.x;
    mouseY = (float)(h)*e.tfinger.y;
    if (mouseX > 0)
    {
        fingerSupport = 1;
    }
    if (pointInRect(mouseX, mouseY, &menuFullScreen->rect))
    {
        full_screen_toggle(window, full_screen);
        full_screen = !full_screen;
    }
}

void interpolateMouse()
{
    mouseX = (float)(w) / w * e.motion.x;
    mouseY = (float)(h) / h * e.motion.y;
}

void handleButtons()
{
    touchButtonEvent();
    SDL_KeyCode mkey = e.key.keysym.sym;
    switch (e.type)
    {
    case SDL_QUIT:
        gQuit = 1;
        break;
    case SDL_KEYDOWN:
    {

        if (mkey == 'f')
        {
            full_screen_toggle(window, full_screen);
            full_screen = !full_screen;
        }
        else if (mkey == 'w' || mkey == 'd' || mkey == 's' || mkey == 'a')
        {
            player->moveByDir(player, mkey, 1);
        }
        else if (mkey == 'b')
        {
            if (player->coin >= 5)
            {
                player->coin = player->coin - 5;
                player->attack = player->attack + 5;
            }
        }
        else if (mkey == 'n')
        {
            int randr = rand() % 10;
            SDL_Rect newrect = {enemyHome->rect.x, enemyHome->rect.y, 5 + randr, 5 + randr};
            Hero *newhero = Hero_new();
            newhero->init(newhero,"enemy2", newrect, 2);
        }
        else if (mkey == 'q')
        {
            gQuit = 1;
        }
        else if (mkey == 'p')
        {
           // playMusic();
        }
        else if (mkey == '[')
        {
            Mix_Pause(1);
        }
        else if (mkey == 'l')
        {
            if (flashButton->enabled == SDL_TRUE)
            {
                flashButton->enabled = SDL_FALSE;
                flashButtonEvent();
            }
        }
        else if (mkey == 'j')
        {
            if (attackButton->enabled == SDL_TRUE)
            {
                attackButton->enabled = SDL_FALSE;
                attackButtonEvent();
            }
        }
        else if (mkey == 'c')
        {
            if (cureButton->enabled == SDL_TRUE)
            {
                cureButton->enabled = SDL_FALSE;
                cureButtonEvent();
            }
        }
        else if (mkey == 'r')
        {
            if (returnHomeButton->enabled == SDL_TRUE)
            {
                returnHomeButton->enabled = SDL_FALSE;
                // returnHomeButtonEvent();
               // Mix_PlayChannel(-1, returnHomeMusic, 0);

                SDL_Thread *t = SDL_CreateThread(waitSecondsTodo, "waitSecondsTodo", 5);
                if (!t)
                {
                    printf("create thread error");
                    return;
                }
            }
        }
    }
    break;
    case SDL_FINGERDOWN:
        interpolateFinger();
        handleDown();
        break;
    case SDL_MOUSEBUTTONDOWN:
        fingerSupport = 0;
        handleDown();
   interpolateMouse();
        break;
    case SDL_MOUSEMOTION:

        break;
    case SDL_MOUSEBUTTONUP:

        break;
    }
}

void drawTouchButton()
{
    touchButton->show(renderer, touchButton);
}
void drawHome()
{
    playerHome->show(renderer, txtText1, font, playerHome);
    char buf[20];
    sprintf(buf, "coin:%d", player->coin);
    txtText1->drawText(renderer, txtText1, font, buf, 240, 10, 1.0f, txtText1->defaultColor);
    enemyHome->show(renderer, txtText1, font, enemyHome);
}

void drawButton()
{
    for (int i = 0; i < pSkillButtonCounts; i++)
    {
        ((pSkillButtons[i]))->show(renderer, (pSkillButtons[i]), txtText1, font);
    }
}
void enemyEvent(Hero *enemy)
{
    if (enemy->hp <= 0)
        return;
    enemyAttackButton->show(renderer, enemyAttackButton, txtText1, font);
    enemy->action(enemy, player->rect);
    if (!pointInCircle(player->rect.x, player->rect.y, enemy->rect.x, enemy->rect.y, enemy->attackRange))
    {

        enemy->move(enemy, 1);
        if (enemy->attackRangeShow == 1)
            enemy->attackRangeShow = 0;
    }
    else
    {

        if (enemyAttackButton->enabled == SDL_TRUE)
        {
            enemy->attackRangeShow = 1;
            enemyAttackEvent();
            enemyAttackButton->enabled = SDL_FALSE;
        }
    }
}
void playerEvent()
{
    drawButton();
    drawHome();
    drawTouchButton();

    if (touchButton->touchStart == SDL_TRUE)
        player->move(player, 1);
    if (pointInRect(player->rect.x, player->rect.y, &playerHome->rect))
    {
        // add player hp
        if (player->hp <= player->hpmax)
        {
            player->hp++;
        }
    }
    if (player->hp <= 0)
    {
        printf("你死了,正在清理尸体...\n");
        returnHomeButtonEvent();
    }
}

void tick()
{
    SDL_RenderClear(renderer);
    while (SDL_PollEvent(&e) != 0)
    {
        handleButtons();
    }

    SDL_SetRenderDrawColor(renderer, 117, 117, 117, 255);
    SDL_RenderClear(renderer);

    // loop all heros
    for (int i = 0; i < pHeroCounts; i++)
    {
       ((pHeros[i]))->show(renderer, (pHeros[i]),txtText1, font, ((pHeros[i]))->type);

        if (((pHeros[i]))->type == 2)
            enemyEvent((pHeros[i]));
    }
    drawHome();
    playerEvent();
    drawMenuFullScreen();
    drawFps();
    SDL_RenderPresent(renderer);
}
void loadMedia()
{
#ifdef android_build

  font = TTF_OpenFont("fzpix.ttf", 15);
//    gameMusic = loadWAV("bgm.ogg");
//    attackMusic = loadWAV("pew.wav");
//    damageMusic = loadWAV("expl3.wav");
//    flashMusic = loadWAV("flash.wav");
//    returnHomeMusic = loadWAV("assets/returnHome.ogg");
#else
    font = TTF_OpenFont("assets/fzpix.ttf", 15);
//    gameMusic = loadWAV("assets/bgm.ogg");
//    attackMusic = loadWAV("assets/pew.wav");
//    damageMusic = loadWAV("assets/expl3.wav");
//    flashMusic = loadWAV("assets/flash.wav");
//    returnHomeMusic = loadWAV("assets/returnHome.ogg");
#endif
   
}
void quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    lw_oopc_delete(touchButton);
    // free heros
    for (int i = 0; i < pHeroCounts; i++)
    lw_oopc_delete(pHeros[i]);

    //     freeHero(pHeros[i]);
    // free skillbuttons
    for (int i = 0; i < pSkillButtonCounts; i++)
    lw_oopc_delete(pSkillButtons[i]);
    lw_oopc_delete(playerHome);
    lw_oopc_delete(enemyHome);
    lw_oopc_delete(txtText1);
    lw_oopc_delete(menuFullScreen);

    
   
}
void gameInit()
{
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect fbrect = {WINDOW_W - MARGIN * 2, WINDOW_H - MARGIN * 2, 80, 80};
    SDL_Rect rthrect = {WINDOW_W - MARGIN * 2 - 100, WINDOW_H - MARGIN * 2, 80, 80};
    SDL_Rect attackrect = {WINDOW_W - MARGIN * 2, WINDOW_H - MARGIN * 2 - 100, 80, 80};
    SDL_Rect curerect = {WINDOW_W - MARGIN * 2 - 200, WINDOW_H - MARGIN * 2, 80, 80};
    SDL_Rect enemyattackrect = {-100, -100, 0, 0};
    flashButton = SkillButton_new();
    flashButton->init(flashButton, "闪现", fbrect, 100, 100, 'l');

    returnHomeButton = SkillButton_new();
    returnHomeButton->init(returnHomeButton, "回城", rthrect, 10, 10, 'r');

    attackButton = SkillButton_new();
    attackButton->init(attackButton, "攻击", attackrect, 10, 10, 'j');

    enemyAttackButton = SkillButton_new();
    enemyAttackButton->init(enemyAttackButton, "enemyAttackButton", enemyattackrect, 10, 10, '-');
    cureButton = SkillButton_new();
    cureButton->init(cureButton, "治疗", curerect, 20, 20, 'c');

    touchButton = TouchButton_new();
    touchButton->init(touchButton, "touchButton");
    touchButton->initByWH(w, h, touchButton);
    SDL_Rect playerRect = {100, 100, 10, 10};
    SDL_Rect enemyRect = {400, 100, 10, 10};
    // player = newHero("player", playerRect, 1);
    // enemy = newHero("enemy", enemyRect, 2); // 2 enemy
    player = Hero_new();
    player->init(player, "player",playerRect, 1);

    enemy = Hero_new();
    enemy->init(enemy, "enemy",enemyRect, 2);

    SDL_Rect phRect = {10, 10, 50, 50};
    SDL_Color phColor = {0, 0, 255, 255};
    // playerHome = newHome("基地", phRect, phColor, SDL_TRUE);
    playerHome = Home_new();
    playerHome->init(playerHome,"基地",phRect, phColor, SDL_TRUE);
    SDL_Rect ehRect = {600, 10, 50, 50};
    SDL_Color ehColor = {255, 0, 0, 255};
    enemyHome = Home_new();
    enemyHome->init(enemyHome,"敌方", ehRect, ehColor, SDL_TRUE);
    menuFullScreen = UiText_new();
    menuFullScreen->init(menuFullScreen,"menuFullScreen", "开全屏",
                               (SDL_Rect){WINDOW_W - MARGIN, MARGIN, 60, 30},
                               SDL_TRUE,
                               'f',
                               (SDL_Color){255, 255, 255, 155},
                               (SDL_Color){0, 0, 0, 200});
    txtText1 = TextTexture_new();
    txtText1->init(txtText1,"基地", "基地", SDL_TRUE);
}
void loop()
{
    startTime = SDL_GetTicks();
#if __EMSCRIPTEN__
        emscripten_set_main_loop(tick, 0, 0);
#else
  while (!gQuit)
    {
       //tick();
//        long current = SDL_GetTicks();
//        long cost = current - startTime;
//        long frame = 1000 / FRAMERATE;
//        long delay = frame - cost;
//        if (delay > 0)
            SDL_Delay(10);
    }
   quit();

#endif
    }

int main(int argc, char *argv[])
{
    if (!init())
    {
        printf("SDL could not be initialized\n");
        return 1;
    }
   gameInit();
   loadMedia();
   loop();

    return 0;
}
