#include "server.h"

#define MAX_SOCKETS 4

Server createTCPServer (Game game) {
    Server s = malloc(sizeof(struct ServerSettings));
    IPaddress ip;
    if (SDLNet_Init() == -1) // initierar nätverk
    {
        printf("SDLNet_Init could not initialize: %s\n", SDLNet_GetError());
        return 0;
    }

    if (SDLNet_ResolveHost(&s->ip, NULL, 1337) == -1) {
        printf("Could not create server %s", SDLNet_GetError());
        return 0; 
    }// server NULL alla nätverks interfaces kommer att accepteras för att etablera koppling 

    s->serverSocket = SDLNet_TCP_Open(&s->ip); // gör anslutningen möjlig 
    if (s->serverSocket == NULL) {
        printf("Could not create server socket %s", SDLNet_GetError());
        return 0; 
    }

    
    s->numberOfSockets = SDLNet_AllocSocketSet(MAX_SOCKETS + 1);
    if (s->numberOfSockets == NULL) {
        printf("Could not allocate socket: %s", SDLNet_GetError());
        return 0;
    }


    if (SDLNet_TCP_AddSocket(s->numberOfSockets, s->serverSocket) == -1) {
        printf("Could not add client sockets to server %s", SDLNet_GetError());
        return 0; 
    }

    return s;
}


int checkSockets(Server s) {
    int num_rdy = SDLNet_CheckSockets(s->numberOfSockets, 1000);
    return num_rdy; 
}


void serverShutDown(Server server) {

    SDLNet_FreeSocketSet(server->numberOfSockets); // frigör sockets

    if(SDLNet_TCP_DelSocket(server->numberOfSockets, server->serverSocket) == -1) {
    printf("Could not delete the set of sockets: %sn", SDLNet_GetError());
    } SDLNet_TCP_Close(server->serverSocket);
    
    SDLNet_Quit();

}