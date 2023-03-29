#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>

#define SOF 		(0xAA)
#define ACC			(0x01)
#define GYRO 		(0x02)
#define MAG 		(0x03)
#define FRAME_SIZE	(7)

typedef struct
{
	uint8_t bSOF;
	uint8_t bSensor;
	uint8_t bEjes;
	uint8_t bEjes2;
	//uint8_t bPayload[4];
	uint8_t bCRC;
}sFrame;

// typedef enum 
// {
// 	GET = 1,
// 	SET,
// }eCMDs;


// typedef enum
// {
// 	ADC = 0,
// 	PUSHBOTTON,
// 	RED,
// 	GREEN,
// 	BLUE,
// 	RGB,
// }eProperties;

int32_t dwfnReadFrame(sFrame * spFrameDest, int32_t dwConnection);
int32_t dwfnWriteFrame(sFrame * spFrameSrc, int32_t dwConnection);

