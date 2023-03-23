

#include "mqtt_frame.h"
#include <stdio.h>


sConnect vfnCreateFrame(char *argv, uint16_t wNum)
{
    sConnect sFrameConnect;
    printf("Hello From vfnCreateFrame");

    sFrameConnect.bFrameType = 0x10;
    sFrameConnect.wMsgLen = wNum;

    return sFrameConnect;
}