#include <stdio.h>
#include "game.h"

int main(int argc, char* argv[]) {
    Game game = createGame();
    updateGame(game);
    exitGame(game);
    return 0;
}