#include "server.h"

typedef struct udpData {
	int idx, x, y, frame;
} udpData;

void initClient(UDPsocket sd, UDPpacket *pReceive, UDPpacket *pSend, Uint32 *clientIP, Uint32 *clientPort, uint8_t *nrOfClients);
    
int main(int argc, char **argv) {
	
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pReceive;       /* Pointer to packet memory */
	UDPpacket *pSend;
    Uint32 client1IP = 0;
    Uint32 client2IP = 0;
    Uint32 client1Port = 0;
    Uint32 client2Port = 0;
	uint8_t nrOfClients = 0;
	bool running = true;

	udpData data = {0, 0, 0};


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
				initClient(sd, pReceive, pSend, &client1IP, &client1Port, &nrOfClients);
            } 
			else if(pReceive->address.port != client1Port && client2IP == 0) {
				initClient(sd, pReceive, pSend, &client2IP, &client2Port, &nrOfClients);
            }
			else {
                if (pReceive->address.port == client1Port){
                    // printf("Received data from client 1\n");
                    if(client2IP != 0){
                        printf("Send to Client 2\n");
                        pSend->address.host = client2IP;	/* Set the destination host */
		                pSend->address.port = client2Port;
                        sscanf((char * )pReceive->data, "%d %d %d %d\n", &data.idx, &data.x, &data.y, &data.frame);
                        printf("x: %d y: %d frame: %d clients %d\n", data.x, data.y, data.frame, nrOfClients);
                        sprintf((char *)pSend->data, "%d %d %d %d %d\n", data.idx, data.x, data.y, data.frame, nrOfClients);
                        pSend->len = strlen((char *)pSend->data) + 1;
                        SDLNet_UDP_Send(sd, -1, pSend);
                    }
                } else if (pReceive->address.port == client2Port){
					printf("Send to Client 1\n");
                    pSend->address.host = client1IP;	/* Set the destination host */
		            pSend->address.port = client1Port;
                    sscanf((char * )pReceive->data, "%d %d %d %d\n", &data.idx, &data.x, &data.y, &data.frame);
					printf("x: %d y: %d frame: %d clients %d\n", data.x, data.y, data.frame, nrOfClients);
                    sprintf((char *)pSend->data, "%d %d %d %d %d\n", data.idx, data.x, data.y, data.frame, nrOfClients);
                    pSend->len = strlen((char *)pSend->data) + 1;
                    SDLNet_UDP_Send(sd, -1, pSend);
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
	SDLNet_Quit();
	return EXIT_SUCCESS;
}

void initClient(UDPsocket sd, UDPpacket *pReceive, UDPpacket *pSend, Uint32 *clientIP, Uint32 *clientPort, uint8_t *nrOfClients) {
	*clientIP = pReceive->address.host;
	*clientPort = pReceive->address.port;

	pSend->address.host = *clientIP;
	pSend->address.port = *clientPort;

	if (*nrOfClients == 0) {
		printf("Client 1\n");
		sprintf((char *)pSend->data, "%d\n", 0);  // Sets client to player 1
	}
	else if (*nrOfClients == 1) {
		printf("Client 2\n");
		sprintf((char *)pSend->data, "%d\n", 1);  // Sets client to player 2
	}

	pSend->len = strlen((char *)pSend->data) + 1;
	SDLNet_UDP_Send(sd, -1, pSend);

	*nrOfClients += 1;
}