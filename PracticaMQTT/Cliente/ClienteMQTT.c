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

#define PORT 5050

pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    //char buffer[1024] = {0};
    //char *hello = "Hello World";
    ssize_t iWriteRc = 0;
    uint8_t* bpInstruction = (uint8_t*)malloc(10);
    sConnect connect_return;

        // FRAME CONNECT

    connect_return = vfnConnect((char *)argv[1], sizeof(argv[1]));

    printf("This is the sFrameType 0x%x \r\n",connect_return.bFRAME_TYPE);
	printf("This is the sFrameType 0x%x \r\n",connect_return.wLEN);
    printf("This is the sFrameType 0x%x \r\n",connect_return.bPROTOCOL_LEVEL);
    printf("This is the sFrameType %x \r\n",connect_return.bCLEAN_SESSION);
    printf("This is the sFrameType 0x%x \r\n",connect_return.bKEEP_ALIVES);
    printf("This is the sFrameType %s \r\n",connect_return.sClient_ID);
    

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }


    iWriteRc = send(sock , (char *)&connect_return ,strlen((char *)&connect_return),0);
    if (iWriteRc <= 0)
    {
        printf("\n Send Failed \n");
        return -1;
    }
    printf("Hello From Client \n");

    while (1) {
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

    // Close socket
    close(sock);

    return 0;
}
