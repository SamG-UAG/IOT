// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
//#include "com.h"
#include "mqtt_frame.h"

#define PORT 1234

pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    //char *hello = "Hello from client";
    //char buffer[1024] = {0};
	ssize_t iWriteRc = 0;
    uint8_t* bpInstruction = (uint8_t*)malloc(10);
	sConnect sReturn;


	printf("argv[] %s , sizeof %d \r\n",argv[1], strlen(argv[1]));

	sReturn = vfnCreateFrame((char *)argv[1], sizeof(argv[1]));
    printf("\n vfnCreateFrame \n");
    printf("This is the sFrameType 0x%x \r\n",sReturn.bFrameType);
	printf("This is the sFrameType 0x%x \r\n",sReturn.wMsgLen);
    printf("This is the sFrameType 0x%x \r\n",sReturn.wProtlNameLen);
    printf("This is the sFrameType %s \r\n",sReturn.sProtName);
    printf("This is the sFrameType 0x%x \r\n",sReturn.bVersion);
    printf("This is the sFrameType 0x%x \r\n",sReturn.bConnectFlags);
    printf("This is the sFrameType 0x%x \r\n",sReturn.bKeepAlive);
    printf("This is the sFrameType 0x%x \r\n",sReturn.wClientIdLen);
    printf("This is the sFrameType %s \r\n",sReturn.sClientID);
    
    /*int socket(int domain, int type, int protocol);
	AF_INET	= IPv4 Internet protocols
	SOCK_STREAM = Provides sequenced, reliable, 
				two-way, connection-based byte streams. 
				An out-of-band data transmission mechanism may be supported.
	Protocol specifies a particular protocol to be used with the socket. 
	Normally only a single protocol exists to support a particular 
	socket type within a given protocol family,
	in which case protocol can be specified as 0.
	*/
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("error \n");
        return -1;
    }
   
   	//struct sockaddr_in 
	//{
	//	sa_family_t    sin_family; /* address family: AF_INET */
	//	in_port_t      sin_port;   /* port in network byte order */
	//	struct in_addr sin_addr;   /* internet address */
	//};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Connection Failed \n");
        return -1;
    }
    // sFrameConnect.bFrameType = 0x10;
    // sFrameConnect.wMsgLen = wNum;
    // sFrameConnect.wProtlNameLen = 0x0004;
    // sFrameConnect.sProtName = 'MATT';
    // sFrameConnect.bVersion = 0x04;
    // sFrameConnect.bConnectFlags = 0x02;
    // sFrameConnect.bKeepAlive = 0x000A;
    // sFrameConnect.wClientIdLen = 0x0005;
    // sFrameConnectsClientID = 'Mario';
    
    iWriteRc = send(sock , (char *)&sReturn ,strlen((char *)&sReturn),0);
    if (iWriteRc <= 0)
    {
        printf("\n Send Failed \n");
        return -1;
    }
    printf("Hello From Client \n");

    while(1)
    {
        fgets((char*)bpInstruction, 10, stdin);
        pthread_mutex_lock(&socketMutex);  
        iWriteRc = send(sock , bpInstruction , strlen((char *)bpInstruction) , 0);
        if (iWriteRc <= 0)
        {
            printf("Shuting down the socket: %d\n", iWriteRc);
        }
        pthread_mutex_unlock(&socketMutex);
        sleep(1);
    }
    return 0;
}

