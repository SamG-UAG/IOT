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
#include <sys/time.h>
#include <signal.h>
#include "com.h"
#include "MQTT_FRAME.h"

#define PORT 5055

pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;

cPingReq Ping_Request;

void timer_handler(int signum){

	static int count = 0;
	printf("timer expired %d times\n", ++count);
}

void *timer_thread(void *arg)
{
    struct sigaction sa;
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &sa, NULL);

    /* Configure the timer to expire after 250 msec... */
    timer.it_value.tv_sec = 40;
    timer.it_value.tv_usec = 0;

    /* ... and every 250 msec after that. */
    timer.it_interval.tv_sec = 40;
    timer.it_interval.tv_usec = 0;
    

    /* Start a virtual timer. It counts down whenever this process is
    executing. */
    setitimer (ITIMER_VIRTUAL, &timer, NULL);

    /* Do busy work. 
    while (1)
    {
    } */

    Ping_Request = vfnRequest();
    printf("\n");
	printf("This is the Message  0x%x \r\n", Ping_Request.bMSG_TYPE);
    printf("This is the Length 0x%d \r\n", Ping_Request.bLENGTH);

    return NULL;
}

void *Publish_Alert(void *argv){
    Publish publish_return;
    int trigger;
    
    scanf("%i", &trigger);

    char message[10] = {"Ayuda"};
    printf("\nalert function 1");
    
    if(trigger == 1){
        publish_return = vfnPublish(message);
        printf("\nalert function 2");

    }
}

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    //char buffer[1024] = {0};
    //char *hello = "Hello World";
    ssize_t iWriteRc = 0;
    uint8_t* bpInstruction = (uint8_t*)malloc(10);
    sConnect connect_return;
    sConnAck *ConnAck_return;
    char bpFramePtr [1024]={0};
    ssize_t ReadStatus = 0;

        // FRAME CONNECT

    connect_return = vfnConnect((char *)argv[1], sizeof(argv[1]));

    printf("This is the bFRAME_TYPE 0x%x \r\n",connect_return.bFRAME_TYPE);
	printf("This is the wLEN 0x%x \r\n",connect_return.wLEN);
    printf("This is the bPROTOCOL_LEVEL 0x%x \r\n",connect_return.bPROTOCOL_LEVEL);
    printf("This is the bCLEAN_SESSION %x \r\n",connect_return.bCLEAN_SESSION);
    printf("This is the bKEEP_ALIVES 0x%x \r\n",connect_return.bKEEP_ALIVES);
    printf("This is the sClient_ID %s \r\n",connect_return.sClient_ID);

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

    pthread_t th_Publish_Alert;
    if(pthread_create (&th_Publish_Alert, NULL, Publish_Alert, NULL)){
            printf("\nalert thread");
    }
    
    iWriteRc = send(sock , (char *)&connect_return ,20,0);
    if (iWriteRc <= 0)
    {
        printf("\n Send Failed \n");
        return -1;
    }
    printf("Hello From Client \n");

    ssize_t Readstatus = read(sock , bpFramePtr, 1024);
    ConnAck_return = (sConnAck *)bpFramePtr;

    printf("This is the bMSG_TYPE 0x%x \r\n",ConnAck_return->bMSG_TYPE);
    printf("This is the bLENGTH 0x%x \r\n",ConnAck_return->bLENGTH);
    printf("This is the bTNCR_RESERVED 0x%x \r\n",ConnAck_return->bTNCR_RESERVED);
    printf("This is the bCONNECT_RETURN 0x%x \r\n",ConnAck_return->bCONNECT_RETURN);


    if(iWriteRc = send(sock , (char *)&Publish_Alert ,20,0)){
        printf("\nSend alert");
    }     

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
