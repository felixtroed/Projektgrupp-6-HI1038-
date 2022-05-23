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
    SDL_Texture *startMenu;
    SDL_Texture *instructionsMenu;
    SDL_Texture *redPlay;
    SDL_Texture *redInstructions;
    SDL_Texture *redQuit;
    SDL_Texture *redBack;
    SDL_Texture *dead;
    SDL_Texture* player1Wins;
    SDL_Texture* player2Wins;
    SDL_Texture* player3Wins;
    SDL_Texture* player4Wins;
    SDL_Rect menuOptionPos[MENUOPTIONS];
    SDL_Rect boxPos;                  //Anv�nds f�r positionen av l�dorna
    SDL_Event event;
    bool accessToServer;
    bool inMenu;
    bool allPlayersDead;
    bool resetInitiated;
    SDL_TimerID resetGameTime;
    SDL_TimerID invincibleTimer;
    int playersDead;

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
    int powerUpCol;
    int powerUpRow;
    int bombDropped;
    int bombPosX;
    int bombPosY;
    int explosionRange;
    int powerUpTaken;
    int leftBoxVal;
    int leftBoxRow;
    int leftBoxCol;
    int rightBoxVal;
    int rightBoxRow;
    int rightBoxCol;
    int topBoxVal;
    int topBoxRow;
    int topBoxCol;
    int bottomBoxVal;
    int bottomBoxRow;
    int bottomBoxCol;
    int explosionDone;
} *udpData;

typedef struct NetworkData {
    UDPsocket sd;
    IPaddress srvAddr;
	UDPpacket *packet1;
    UDPpacket *packet2;
    char inputIPAddress[30];
    bool willSend;
} *Network;

Game createGame();
void updateGame(Game game, Network net, udpData packetData);
void exitGame(Game game, Network net, udpData packetData);
Network createNet();
udpData createPacketData(Game game);
bool loadTextures(SDL_Renderer** renderer, SDL_Surface** bitmapSurface, SDL_Texture** texture, char pictureDestination[64]);

#endif /* GAME_H */