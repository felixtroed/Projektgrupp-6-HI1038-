#ifndef GAME_H
#define GAME_H

// #include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>
#include "player.h"
#include "box.h"
#include "bomb.h"
#include "gameLogic.h"

#define WINDOW_WIDTH 1088
#define WINDOW_HEIGHT 832
#define BOMBS 12
#define MENUOPTIONS 4
#define NUMPLAYERS 4

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
    //Boxes boxes;
    Player player[NUMPLAYERS];
    Bomb bombs[BOMBS];                // Contains all simultaneously allowed bombs
    uint8_t nrOfPlayers;
    uint8_t pIdx;
} *Game;

typedef struct NetworkData {
    UDPsocket sd;
    IPaddress srvAddr;
	UDPpacket *packet1;
    UDPpacket *packet2;
} *Network;

Game createGame(Network net);
void updateGame(Game game, Network net);
void exitGame(Game game, Network net);
Network createNet();

#endif /* GAME_H */