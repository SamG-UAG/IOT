#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>


typedef struct
{
    uint8_t bFrameType;     //0x10
    uint16_t wMsgLen;
    uint16_t wProtlNameLen; //0x0004
    char sProtName [100];   //String MQTT
    uint8_t bVersion;       //0x04
    uint8_t bConnectFlags;  //0x02
    uint16_t bKeepAlive;
    uint16_t wClientIdLen;
    char sClientID [100];
}sConnect;


sConnect vfnCreateFrame(char *argv, uint16_t bNum);