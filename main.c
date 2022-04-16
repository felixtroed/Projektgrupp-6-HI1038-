#include <stdio.h>
#include "game.h"
#include "box.h"

int main(int argc, char* argv[]) {
    Game game = createGame();
    Player player = makePlayer();
    updateGame(game,player);
    exitGame(game);
    return 0;
}