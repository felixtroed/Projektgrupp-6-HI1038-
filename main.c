#include <stdio.h>
#include "game.h"
#include "player.h"

int main(int argc, char* argv[]) {
    Game game = createGame();
    Network net = createNet();
    updateGame(game, net);
    exitGame(game, net);
    return 0;
}