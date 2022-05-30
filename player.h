#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

typedef struct GameSettings* Game;
typedef struct PlayerSettings* Player;

Player createPlayer(int playerNumber, int x, int y, Game game);
void renderPlayers(Game game);
bool playerIsAlive(Player player);
bool bombsAvailable(Player player);
void decrementBombsAvailable(Player player);
int getPlayerPosX(Player player);
int getPlayerPosY(Player player);
int getPlayerExpRange(Player player);
uint8_t* getBombsAvailableVal(Player player);
void setPlayerPosX(Player player, int x);
void setPlayerPosY(Player player, int y);
void setPlayerFrame(Player player, int frame);
void setPlayerExpRange(Player player, int explosionRange);
void setPlayerToHurt(Player player);
void setPlayerToNotHurt(Player player);
void setPlayerToDead(Player player);
void setPlayerToAlive(Player player);
void setToLastPlayer(Player player);
void setToNotLastPlayer(Player player);
bool playerIsHurt(Player player);
void modifyHitboxPos(Player player);
SDL_Rect* getPlayerHitboxPos(Player player);
void decrementPlayerLives(Player player);
bool playerHasLivesRemaining(Player player);
bool playerIsLastPlayer(Player player);
void setPlayerLives(Player player, uint8_t lives);
void setBombsAvailable(Player player, uint8_t bombs);
void incrementBombsAvailable(Player player);
void setPlayerSpeed(Player player, int speed);
void incrementPlayerSpeed(Player player);
int getPlayerSpeed(Player player);
void destroyPlayerTexture(Player player);
int getMaxBombs(Player player);
void incrementMaxBombs(Player player);
void incrementPlayerExpRange(Player player);
void increasePlayerPosY(Player player);
void increasePlayerPosX(Player player);
void decreasePlayerPosY(Player player);
void decreasePlayerPosX(Player player);
void incrementPlayerFrame(Player player);
int getPlayerFrame(Player player);
int getPlayerHeight();
int getPlayerWidth();

#endif /* PLAYER_H */