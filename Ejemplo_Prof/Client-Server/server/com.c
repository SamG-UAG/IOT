#include "com.h"
const uint8_t baInstructions[][20] = 
{
	{"getSensor"}
};

uint8_t bfnInstructionDiscover(uint8_t* bpInstruction);

ssize_t dwfnReadFrame(sFrame * spFrameDest, int32_t dwConnection)
{
	char bpFramePtr [1024]={0};
	//uint8_t dwIndex = 0;
	uint8_t *bpData;

	ssize_t Readstatus = read( dwConnection , bpFramePtr, 1024);
#if DEBUG == 1
	printf("dwfnReadFrame\n\r");
#endif
	bpData = (uint8_t *)&bpFramePtr;
	printf("%s", bpFramePtr);
	if(bfnInstructionDiscover(bpData))
	{

	}
	return Readstatus;
}

int32_t dwfnWriteFrame(sFrame * spFrameSrc, int32_t dwConnection)
{
	uint8_t* bpFramePtr;
	bpFramePtr = (uint8_t*)malloc(FRAME_SIZE);
	//*bpFramePtr = SOF;
	//bpFramePtr++;
	memcpy(bpFramePtr,spFrameSrc,sizeof(FRAME_SIZE));
	if((write(dwConnection,bpFramePtr,FRAME_SIZE)) != FRAME_SIZE)
	{
		printf("Writing Error\n\r");
		//bpFramePtr--;
		free(bpFramePtr);	
		return -1;
	}
	else
	{
		//bpFramePtr--;
		free(bpFramePtr);
		return 0;
	}

}


uint8_t bfnInstructionDiscover(uint8_t* bpInstruction)
{
	if(!memcmp(bpInstruction,(uint8_t*)&baInstructions[0][0],6))
	{
		printf("\r\n Well Done!!!\n\r");
		return 1;
	}
	return 0;
}