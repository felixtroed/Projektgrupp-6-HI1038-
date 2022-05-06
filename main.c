#include <stdio.h>
#include "game.h"
#include "player.h"

int main(int argc, char* argv[]) {
    Network net = createNet();
    Game game = createGame();
    udpData packetData = createPacketData(game);
    updateGame(game, net, packetData);
    exitGame(game, net, packetData);
    return 0;
}