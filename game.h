#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef GAME_H
#define GAME_H

typedef struct GameSettings *Game;

Game createGame();
void updateGame(Game game);
void exitGame(Game game);

#endif /* GAME_H */