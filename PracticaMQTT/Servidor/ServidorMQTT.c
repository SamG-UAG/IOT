#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>
#include "com.h"

#include "MQTT_FRAME.h"


#define DEBUG 1

#define CONNECT_PORT 5055 //Port used for the next connection
#define MAXSOCKETS   10   //Limit of sockets.

//Global variables used by ptheads
uint8_t listSocket[MAXSOCKETS]={0};
int socketIndex;
pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;
int newSocketMember = 0;

void ServerClientThread(void * vpSocketTemp);

int main(int argc , char *argv[])
{
	int32_t dwServerSocket;
	int32_t dwSize;
	int32_t dwCurConnection;
	struct sockaddr_in sServSettings;
	struct sockaddr_in sClient;
	pthread_t ClientT;

	sConnect sReturn;
	printf("argv[] %s, sizeof %d \r\n",argv[1], strlen(argv[1]));

	sReturn = vfnConnect(argv[1], sizeof(argv[1]));

	printf("Server Init\n\r");
	//Set IP information
	sServSettings.sin_family = AF_INET;
	sServSettings.sin_addr.s_addr = INADDR_ANY;
	sServSettings.sin_port = htons(CONNECT_PORT);	
	
 	dwServerSocket = socket(AF_INET,SOCK_STREAM,0);
 	if(dwServerSocket < 0)
 	{
 		printf("Error Creating socket\n\r");
 	}
	printf("Binding ...\n\r");
 	if(bind(dwServerSocket, (struct sockaddr *)&sServSettings,sizeof(sServSettings)))
 	{
 		printf("Error Binding\n\r");
 	}
	printf("Listening ...\n\r");
 	listen(dwServerSocket, 10);
 	dwSize = sizeof(struct sockaddr_in);

	pthread_create(&ClientT, NULL, (void*) &ServerClientThread, (void *) &dwCurConnection);
	socketIndex=0;	
	
	while (1)
 	{
		if ((dwCurConnection = accept(dwServerSocket,(struct sockaddr*)&sClient,
								(socklen_t*)&dwSize))<0)
		{
			printf("Accepting Error\n\r");
			printf("There was an error traying to accept in the socket, errno:%d\n", errno);
			continue;

		}
		else
		{
			printf("Connection Accept ...\n\r");

		}

		if (listSocket[socketIndex] == 0)
		{
			listSocket[socketIndex] = dwCurConnection;
			printf("adding new connection listSocket[%d] = %d\n", socketIndex, listSocket[socketIndex]);
			socketIndex++;
		}
		else
		{
			continue;
		}
		
		sleep(1);
	}
	return 0;
}

void ServerClientThread(void * vpSocketTemp)
{
	int i = 0;
	int32_t* dwpSocket;
	uint8_t bNumsocketIndex = 0;
	ssize_t ReadStatus = 0;
	sFrame sFrameInfo;
	dwpSocket = (int32_t *) vpSocketTemp;
	char bpFramePtr [1024]={0};
	sConnAck ConnAck_return;

	sConnect *Client_Conn_Frame;
	sFrameInfo.bSOF = 0x31;
	sFrameInfo.bCMD= 0x35;

	Publish *publish_frame;

	//printf("Prueba thread");

    while (1)
    {
		//printf("Prueba ciclo while");

        pthread_mutex_lock(&socketMutex);    
        //Send a messages to all the members connected.
        for (bNumsocketIndex = 0; bNumsocketIndex < 10; bNumsocketIndex++)
        {
			//printf("Prueba ciclo for");
			if (listSocket[bNumsocketIndex] != 0)
			{
			//Write the message in the socket.
			//ReadStatus = dwfnReadFrame(&sFrameInfo, *dwpSocket);

			//printf("Prueba if");

			ssize_t Readstatus = read( *dwpSocket , bpFramePtr, 1024);
			
			

			/*printf("This is the bFRAME_TYPE 0x%x \r\n",Client_Conn_Frame->bFRAME_TYPE);
			printf("This is the wLEN 0x%x \r\n",Client_Conn_Frame->wLEN);
			printf("This is the bPROTOCOL_LEVEL 0x%x \r\n",Client_Conn_Frame->bPROTOCOL_LEVEL);
			printf("This is the bCLEAN_SESSION %x \r\n",Client_Conn_Frame->bCLEAN_SESSION);
			printf("This is the bKEEP_ALIVES 0x%x \r\n",Client_Conn_Frame->bKEEP_ALIVES);
			printf("This is the sClient_ID %s \r\n",Client_Conn_Frame->sClient_ID);

			printf("The Client ID is 0%x \r\n", Client_Conn_Frame->bFRAME_TYPE);
			printf("This is the Rx Frame 0%x \r\n",*bpFramePtr);*/
			if(*bpFramePtr == 0x10){
				Client_Conn_Frame = (sConnect *)bpFramePtr;
				printf("1");
				if(Client_Conn_Frame->wLEN > 0){
					printf("2");
					if(Client_Conn_Frame->bPROTOCOL_LEVEL == 0x0004){
						printf("3");
						if(Client_Conn_Frame->bCLEAN_SESSION == 0x02){
							printf("4 \n");
							if(Client_Conn_Frame->bKEEP_ALIVES == 0x000A){
								printf("Conection Acknowledge\n");
								ConnAck_return = vfnConnAck (Client_Conn_Frame->sClient_ID, sizeof(Client_Conn_Frame->sClient_ID));
								
								printf("This is the bMSG_TYPE 0x%x \r\n",ConnAck_return.bMSG_TYPE);
								printf("This is the bLENGTH 0x%x \r\n",ConnAck_return.bLENGTH);
								printf("This is the bTNCR_RESERVED 0x%x \r\n",ConnAck_return.bTNCR_RESERVED);
								printf("This is the bCONNECT_RETURN 0x%x \r\n",ConnAck_return.bCONNECT_RETURN);

								send(*dwpSocket , (char *)&ConnAck_return ,20,0);
							}
						}
					}	
				}
			}
			else if(*bpFramePtr == 0x30)
			{
				publish_frame = (Publish *)bpFramePtr;
				printf("Client Publish Message: ", publish_frame->MESSAGE);
				/*for(i=0; i<MAXSOCKETS; i++){
					if(listSocket != 0){
						send(*dwpSocket , (char *)&publish_frame ,20,0);
					}
				} */
			}
			else{

			}
 

#if DEBUG == 1
			printf("bNumsocketIndex : %d\n",bNumsocketIndex);
			printf("Reading to: %d\n",listSocket[bNumsocketIndex]);
			printf("ReadStatus : %d\n",ReadStatus);
#endif
			//If there was an error writing to the socket then
			//we shutdown the socket to let other the connection.
			if (ReadStatus < 0)
			{
				printf("Shuting down a socket of index: %d\n", bNumsocketIndex);
				shutdown(listSocket[socketIndex], SHUT_RDWR);
				listSocket[bNumsocketIndex] = 0;
			}
          }
        }
        pthread_mutex_unlock(&socketMutex);
        //This pthread must sleep for 10 seconds.
        sleep(1);      
    }
}