#include <string.h>

typedef struct{
    uint8_t bFRAME_TYPE;
    uint16_t wLEN;
    char sClient_ID [];
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

sConnect vfnCreateFrame(char *argv, uint16_t wNum);
sConnAck vfnConnAck(char *argv, uint8_t Connection);
cPingReq vfnRequest();
sPingResp vfnResponse();
