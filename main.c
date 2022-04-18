#include <stdio.h>
#include "game.h"
#include "player.h"

int main(int argc, char* argv[]) {
    Game game = createGame();
    Player p1 = createPlayer(64, 64, game);
    updateGame(game, p1);
    exitGame(game);
    return 0;
}