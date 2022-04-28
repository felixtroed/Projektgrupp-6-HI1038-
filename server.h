#ifndef SERVER_H
#define SERVER_H
#include <SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#define MAXSOCKETS 4

typedef struct GameSettings* Game;
typedef struct ServerSettings {
	SDLNet_SocketSet numberOfSockets;
	TCPsocket serverSocket; 
	IPaddress ip;
	TCPsocket sockets[MAXSOCKETS];
} *Server; 

Server createTCPServer(Game game);
int checkSockets(Server s);
void serverShutDown(Server server);

#endif /* SERVER_H*/