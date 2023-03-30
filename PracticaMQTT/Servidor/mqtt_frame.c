#include "MQTT_FRAME.h"
#include<stdio.h>

sConnect vfnConnect(char *argv, uint16_t wNum){
    sConnect sConnect_Frame;

    strcpy(sConnect_Frame.sClient_ID, argv);    
    sConnect_Frame.bFRAME_TYPE = 0x10;
    sConnect_Frame.wLEN = wNum;
    sConnect_Frame.bPROTOCOL_LEVEL = 0x0004;
    sConnect_Frame.bCLEAN_SESSION = 0x02;
    sConnect_Frame.bKEEP_ALIVES = 0x000A;

    return sConnect_Frame;
}

sConnAck vfnConnAck(char *argv, uint8_t Connection){
    sConnAck ConnAck_Frame;

    ConnAck_Frame.bMSG_TYPE = 0x20;
    ConnAck_Frame.bLENGTH = 0x02;
    ConnAck_Frame.bTNCR_RESERVED = 0x00;
    ConnAck_Frame.bCONNECT_RETURN = Connection;

    return ConnAck_Frame;
}

cPingReq vfnRequest(){
    cPingReq Request_Frame;

    Request_Frame.bMSG_TYPE = 0xC0;
    Request_Frame.bLENGTH = 0x00;

    return Request_Frame;
}

sPingResp vfnRespon(){
    sPingResp Response_Frame;

    Response_Frame.bMSG_TYPE = 0xD0;
    Response_Frame.bLENGTH = 0x00;

    return Response_Frame;
}