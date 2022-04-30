#ifndef GAME_H
#define GAME_H

// #include <SDL.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "player.h"
#include "box.h"
#include "bomb.h"
#include "gameLogic.h"

#define WINDOW_WIDTH 1088
#define WINDOW_HEIGHT 832
#define BOMBS 4
#define MENUOPTIONS 4

typedef struct PlayerSettings* Player;
typedef struct BombSettings* Bomb;
typedef struct BoxInit* Boxes;


typedef struct GameSettings {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *bitmapSurface;         //Anv�nds f�r ladda upp bilder
    SDL_Texture *background;             //Anv�nds f�r att ladda in bilden av bakgrunden
    SDL_Texture *box;                   //Anv�nds f�r att ladda in bilden av l�dan
    SDL_Texture *startMenu;
    SDL_Texture *instructionsMenu;
    SDL_Texture *redPlay;
    SDL_Texture *redInstructions;
    SDL_Texture *redQuit;
    SDL_Texture *redBack;
    SDL_Rect menuOptionPos[MENUOPTIONS];
    SDL_Rect boxPos;                  //Anv�nds f�r positionen av l�dorna
    SDL_Event event;
    Boxes boxes;
    Player p1;
    Player p2;
    Player p3;
    Player p4;
    Bomb bombs[BOMBS];                // Contains all simultaneously allowed bombs
} *Game;

Game createGame();
void updateGame(Game game);
void exitGame(Game game);

#endif /* GAME_H */