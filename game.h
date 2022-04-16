#include <SDL.h>
#include "player.h"

#ifndef GAME_H
#define GAME_H

typedef struct GameSettings *Game;

typedef struct PlayerSettings* Player;

Game createGame();
Player makePlayer(); 

void setBackground(game);
void updateGame(Game game, Player player);
void setUpTheRest(Game game, Player player);
void exitGame(Game game);

#endif /* GAME_H */