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
	uint8_t amt_wood;
	uint32_t timer_wood;
	int socketIndex;
	int in_use;
	int questing;
	
} *Server; 

Server createTCPServer(Game game);
int checkSockets(Server s);
void serverShutDown(Server server);

#endif /* SERVER_H*/