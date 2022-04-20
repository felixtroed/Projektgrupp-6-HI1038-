#ifndef GAME_H
#define GAME_H

// #include <SDL.h>
#include <SDL2/SDL.h>
#include "player.h"
#include "box.h"
#include "bomb.h"
#include "gameLogic.h"

#define BOMBS 4

typedef struct PlayerSettings* Player;
typedef struct Bomb* Bomb;

typedef struct GameSettings {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *bitmapSurface;         //Anv�nds f�r ladda upp bilder
    SDL_Texture *background;             //Anv�nds f�r att ladda in bilden av bakgrunden
    SDL_Texture *box;                   //Anv�nds f�r att ladda in bilden av l�dan
    SDL_Rect boxPos;                  //Anv�nds f�r positionen av l�dorna
    SDL_Event event;
    Player p1;
    Bomb bombs[BOMBS];                // Contains all simultaneously allowed bombs
} *Game;

Game createGame();
void updateGame(Game game);
void exitGame(Game game);

#endif /* GAME_H */