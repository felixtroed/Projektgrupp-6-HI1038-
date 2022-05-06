#ifndef GAME_H
#define GAME_H

#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_net.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_net.h>
#include <string.h>
#include "player.h"
#include "box.h"
#include "bomb.h"
#include "gameLogic.h"
#include "powerUps.h"

#define WINDOW_WIDTH 1088
#define WINDOW_HEIGHT 832
#define BOMBS 20
#define MENUOPTIONS 4
#define NUMPLAYERS 4

typedef struct PlayerSettings* Player;
typedef struct BombSettings* Bomb;
typedef struct BoxInit* Boxes;
typedef struct PowerUPS* PowerUPS;

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
    SDL_Texture *dead;
    Boxes boxes;
    PowerUPS power;
    Player player[NUMPLAYERS];
    Bomb bombs[BOMBS];                // Contains all simultaneously allowed bombs
    uint8_t activePlayers;
    uint8_t pIdx;

} *Game;

typedef struct udpData {
    int pIdx;
    int xPos;
    int yPos;
    int frame;
    int isHurt;
    int isDead;
    int powerCol;
    int powerRow;
    int PowerUpGone;
    int bombDropped;
    int bombPosX;
    int bombPosY;
    int explosionRange;
    int powerUps[ROW_SIZE][COLUMN_SIZE];
    int dataInit;
    int length;
} *udpData;


typedef struct udpInit {
    int powerUps[ROW_SIZE][COLUMN_SIZE];
    int dataInit;
}*udpInit;

typedef struct NetworkData {
    UDPsocket sd;
    IPaddress srvAddr;
    UDPpacket *packet0; 
	UDPpacket *packet1;
    UDPpacket *packet2;
    bool willSend;
    bool boxGone;
} *Network;

Game createGame(Network net);
void updateGame(Game game, Network net, udpData packetData);
void exitGame(Game game, Network net, udpData packetData);
Network createNet();
udpData createPacketData(Game game);
bool loadTextures(SDL_Renderer** renderer, SDL_Surface** bitmapSurface, SDL_Texture** texture, char pictureDestination[64]);

#endif /* GAME_H */