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

Publish vfnPublish(char *argv){
    Publish Publish_frame;
    
    Publish_frame.bMSG_TYPE = 0x30;
    Publish_frame.TOPIC_LENGTH = 0x07;
    strcpy(Publish_frame.TOPIC_NAME, "alarma1");
    Publish_frame.MSG_LENGTH = 0x05;
    strcpy(Publish_frame.MESSAGE, argv);

    return Publish_frame;
}

Subscribe vfnSubscribe(char *argv){
    Subscribe Subscribe_frame;

    Subscribe_frame.bMSG_TYPE = 0x80;
    Subscribe_frame.TOPIC_LENGTH = 0x07;
    strcpy(Subscribe_frame.TOPIC_NAME, argv);

    return Subscribe_frame;
}