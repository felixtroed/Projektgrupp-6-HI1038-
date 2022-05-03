#include <stdio.h>
#include "game.h"
#include "player.h"

int main(int argc, char* argv[]) {
    Network net = createNet();
    Game game = createGame(net);
    updateGame(game, net);
    exitGame(game, net);
    return 0;
}