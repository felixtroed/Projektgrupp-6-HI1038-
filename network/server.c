#include "server.h"

int main(int argc, char **argv) {
	
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *pReceive;       /* Pointer to packet memory */
	UDPpacket *pSent;
    Uint32 client1IP = 0; 
    Uint32 client2IP = 0;
    Uint32 client1Port = 0; 
    Uint32 client2Port = 0;
	bool isRunning = 1;
    int x, y;

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
	if (!((pSent = SDLNet_AllocPacket(512)) && (pReceive = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Main loop */
	while (isRunning)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd, pReceive))
		{
			//printf("UDP Packet incoming\n");
			//printf("\tData:    %s\n", (char *)pReceive->data);
			//printf("\tAddress: %x %x\n", pReceive->address.host, pReceive->address.port);
            if(client1IP == 0 && client1Port == 0){
                printf("Client 1\n");
                client1IP = pReceive->address.host;
                client1Port = pReceive->address.port;
            }else if(pReceive->address.port != client1Port  && client2IP == 0){
                printf("Client 2\n");
                client2IP = pReceive->address.host;
                client2Port = pReceive->address.port;
            }else{
                if (pReceive->address.port == client1Port){
                    printf("Received data\n");
                    if(client2IP != 0){
                        printf("Send to Client 2\n");
                        pSent->address.host = client2IP;	/* Set the destination host */
		                pSent->address.port = client2Port;
                        sscanf((char * )pReceive->data, "%d %d\n", &x, &y);
                        printf("%d %d\n", x, y);
                        sprintf((char *)pSent->data, "%d %d\n", x,  y);
                        pSent->len = strlen((char *)pSent->data) + 1;
                        SDLNet_UDP_Send(sd, -1, pSent);
                    }
                } else if (pReceive->address.port == client2Port){
                    printf("Send to Client 1\n");    
                    pSent->address.host = client1IP;	/* Set the destination host */
		            pSent->address.port = client1Port;
                    sscanf((char * )pReceive->data, "%d %d\n", &x, &y);
                    printf("%d %d\n", x, y);
                    sprintf((char *)pSent->data, "%d %d\n", x, y);
                    pSent->len = strlen((char *)pSent->data) + 1;
                    SDLNet_UDP_Send(sd, -1, pSent);
                }
                
            }
            

			/* Quit if packet contains "quit" */
			if (strcmp((char *)pSent->data, "quit") == 0)
				isRunning = false;
		}		
	}

	/* Clean and exit */
	SDLNet_FreePacket(pSent);
    SDLNet_FreePacket(pReceive);
	SDLNet_Quit();
	return EXIT_SUCCESS;
} 