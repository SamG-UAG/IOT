

#include "mqtt_frame.h"
#include <stdio.h>


sConnect vfnCreateFrame(char *argv, uint16_t wNum)
{
    sConnect sFrameConnect;
    printf("Hello From vfnCreateFrame");

    sFrameConnect.bFrameType = 0x31;
    sFrameConnect.wMsgLen = 0x32;//wNum;
    sFrameConnect.wProtlNameLen = 0x0034;
    //Use String Copy
    sFrameConnect.sProtName [0]= 'M';
    sFrameConnect.sProtName [1]= 'Q';
    sFrameConnect.sProtName [2]= 'T';
    sFrameConnect.sProtName [3]= 'T';
    sFrameConnect.bVersion = 0x04;
    sFrameConnect.bConnectFlags = 0x02;
    sFrameConnect.bKeepAlive = 0x000A;
    sFrameConnect.wClientIdLen = 0x0005;
    //Use String Copy
    sFrameConnect.sClientID [0] = 'M';
    sFrameConnect.sClientID [1] = 'A';
    sFrameConnect.sClientID [2] = 'R';
    sFrameConnect.sClientID [3] = 'I';
    sFrameConnect.sClientID [4] = 'O';

    return sFrameConnect;
}