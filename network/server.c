#include "server.h"

typedef struct udpData {
	int idx, x, y, frame, nrOfClients, isHurt, isDead, powerUpCol, powerUpRow, boxValue, bombDropped, bombPosX, bombPosY, explosionRange,
	leftBoxVal, leftBoxRow, leftBoxCol, powerUpTaken, rightBoxVal, rightBoxRow, rightBoxCol, topBoxVal, topBoxRow, topBoxCol, bottomBoxVal, bottomBoxRow, bottomBoxCol;
} *udpData;

void initClient(UDPsocket sd, UDPpacket *pReceive, UDPpacket *pSend, Uint32 *clientIP, Uint32 *clientPort, udpData data);
void sendData(UDPsocket sd, UDPpacket *pReceive, UDPpacket *pSend, udpData data, Uint32 clientIP, Uint32 clientPort);

int main(int argc, char **argv) {
	
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pReceive;       /* Pointer to packet memory */
	UDPpacket *pSend;
    Uint32 client1IP = 0;
    Uint32 client2IP = 0;
    Uint32 client3IP = 0;
    Uint32 client4IP = 0;
    Uint32 client1Port = 0;
    Uint32 client2Port = 0;
    Uint32 client3Port = 0;
    Uint32 client4Port = 0;
	bool running = true;

	udpData data = malloc(sizeof(struct udpData));
	data->idx = 0;
	data->x = 0;
	data->y = 0;
	data->frame = 0;
	data->nrOfClients = 0;
	data->powerUpCol = 0;
	data->powerUpRow = 0;
	data->powerUpTaken = 0;


	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(2000)))
	{
		fprintf(stderr, "SDLNet_UDP_Open Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Make space for the packet */
	if (!((pSend = SDLNet_AllocPacket(512)) && (pReceive = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	printf("Server initiated\n");


	/* Main loop */
	while (running) {

		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pReceive)) {

            if(client1IP == 0 && client1Port == 0) {
				initClient(sd, pReceive, pSend, &client1IP, &client1Port, data);
            } 
			else if(pReceive->address.port != client1Port && client2IP == 0) {
				initClient(sd, pReceive, pSend, &client2IP, &client2Port, data);
            }
			else if(pReceive->address.port != client1Port && pReceive->address.port != client2Port && client3IP == 0) {
				initClient(sd, pReceive, pSend, &client3IP, &client3Port, data);
            }
			else if(pReceive->address.port != client1Port && pReceive->address.port != client2Port && pReceive->address.port != client3Port && client4IP == 0) {
				initClient(sd, pReceive, pSend, &client4IP, &client4Port, data);
            }
			else {
                if (pReceive->address.port == client1Port) {
                    if(client2IP != 0) {
						sendData(sd, pReceive, pSend, data, client2IP, client2Port);
                    }
					if(client3IP != 0) {
                        sendData(sd, pReceive, pSend, data, client3IP, client3Port);
                    }
					if(client4IP != 0) {
						sendData(sd, pReceive, pSend, data, client4IP, client4Port);
                    }
                }
				else if (pReceive->address.port == client2Port) {
					sendData(sd, pReceive, pSend, data, client1IP, client1Port);

					if(client3IP != 0) {
						sendData(sd, pReceive, pSend, data, client3IP, client3Port);
					}
					if(client4IP != 0) {
						sendData(sd, pReceive, pSend, data, client4IP, client4Port);
                    }
                }

				else if (pReceive->address.port == client3Port) {
                    sendData(sd, pReceive, pSend, data, client1IP, client1Port);
					sendData(sd, pReceive, pSend, data, client2IP, client2Port);

					if(client4IP != 0) {
						sendData(sd, pReceive, pSend, data, client4IP, client4Port);
                    }
                }

				else if (pReceive->address.port == client4Port) {
                    sendData(sd, pReceive, pSend, data, client1IP, client1Port);
					sendData(sd, pReceive, pSend, data, client2IP, client2Port);
					sendData(sd, pReceive, pSend, data, client3IP, client3Port);
                }
            }
            

			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSend->data, "quit") == 0)
				running = false;
		}
	}

	/* Clean and exit */
	SDLNet_FreePacket(pSend);
    SDLNet_FreePacket(pReceive);
	free(data);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}

void sendData(UDPsocket sd, UDPpacket *pReceive, UDPpacket *pSend, udpData data, Uint32 clientIP, Uint32 clientPort) {
	pSend->address.host = clientIP;		/* Set the destination host */
	pSend->address.port = clientPort;
	sscanf((char * )pReceive->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", 
		&data->idx, &data->x, &data->y, 
		&data->frame, &data->isHurt, &data->isDead,
		&data->powerUpRow, &data->powerUpCol, &data->bombDropped,
		&data->bombPosX, &data->bombPosY, &data->explosionRange,
		&data->leftBoxVal, &data->leftBoxRow, &data->leftBoxCol,
		&data->powerUpTaken, &data->rightBoxVal, &data->rightBoxRow, &data->rightBoxCol,
		&data->topBoxVal, &data->topBoxRow, &data->topBoxCol,
		&data->bottomBoxVal, &data->bottomBoxRow, &data->bottomBoxCol
	);
	// printf("powerUpCol: %d boxRow: %d \n", data->powerUpCol, data->boxRow);
	sprintf((char *)pSend->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", data->idx, data->x, data->y, data->frame, data->nrOfClients, data->isHurt, data->isDead, data->powerUpRow, data->powerUpCol, data->bombDropped, data->bombPosX, data->bombPosY, data->explosionRange, data->leftBoxVal, data->leftBoxRow, data->leftBoxCol, data->powerUpTaken, data->rightBoxVal, data->rightBoxRow, data->rightBoxCol, data->topBoxVal, data->topBoxRow, data->topBoxCol, data->bottomBoxVal, data->bottomBoxRow, data->bottomBoxCol);
	pSend->len = strlen((char *)pSend->data) + 1;
	SDLNet_UDP_Send(sd, -1, pSend);
}

void initClient(UDPsocket sd, UDPpacket *pReceive, UDPpacket *pSend, Uint32 *clientIP, Uint32 *clientPort, udpData data) {
	*clientIP = pReceive->address.host;
	*clientPort = pReceive->address.port;

	pSend->address.host = *clientIP;
	pSend->address.port = *clientPort;
	if (data->nrOfClients == 0) {
		printf("Client 1\n");
		sprintf((char *)pSend->data, "%d\n", 0);  // Sets client to player 1
	}
	else if (data->nrOfClients == 1) {
		printf("Client 2\n");
		sprintf((char *)pSend->data, "%d\n", 1);  // Sets client to player 2
	}
	else if (data->nrOfClients == 2) {
		printf("Client 3\n");
		sprintf((char *)pSend->data, "%d\n", 2);  // Sets client to player 3
	}
	else if (data->nrOfClients == 3) {
		printf("Client 4\n");
		sprintf((char *)pSend->data, "%d\n", 3);  // Sets client to player 4
	}

	pSend->len = strlen((char *)pSend->data) + 1;
	SDLNet_UDP_Send(sd, -1, pSend);

	data->nrOfClients += 1;
}