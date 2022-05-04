#include <stdio.h>
#include "game.h"
#include "player.h"

int main(int argc, char* argv[]) {
    Network net = createNet();
    Game game = createGame(net);
    udpData packetData = createPacketData(game->pIdx);
    updateGame(game, net, packetData);
    exitGame(game, net, packetData);
    return 0;
}