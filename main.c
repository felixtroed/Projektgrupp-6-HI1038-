#include <stdio.h>
#include "game.h"
#include "player.h"

int main(int argc, char* argv[]) {
    /*Network net creates a struct for network settings allocates memory in heap*/
    Network net = createNet();  
    /*Game */
    Game game = createGame();     
    /*Creates UDPpacketData */
    udpData packetData = createPacketData(game);
    /*Game loop */
    updateGame(game, net, packetData);
    /*Frees everything */
    exitGame(game, net, packetData);
    return 0;
}