#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>

typedef struct {
    uint8_t bFRAME_TYPE;
    uint16_t wLEN;
    char sClient_ID [30];
    uint8_t bPROTOCOL_LEVEL;
    uint8_t bCLEAN_SESSION;
    uint16_t bKEEP_ALIVES;
}sConnect;

typedef struct{
    uint8_t bMSG_TYPE;
    uint8_t bLENGTH;
    uint8_t bTNCR_RESERVED;
    uint8_t bCONNECT_RETURN;
}sConnAck;

typedef struct{
    uint8_t bMSG_TYPE;
    uint8_t bLENGTH;
}sPingResp;

typedef struct{
    uint8_t bMSG_TYPE;
    uint8_t bLENGTH;
}cPingReq;

sConnect vfnConnect(char *argv, uint16_t wNum);
sConnAck vfnConnAck(char *argv, uint8_t Connection);
cPingReq vfnRequest();
sPingResp vfnResponse();
