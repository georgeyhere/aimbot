#include "OCM.h"

/***
 * uint8_t getMemFlag
 *
 * @param  flagAddr is a pointer to the address of the shared memory flag
 * @return none
 * @notes  none
 ***/
uint32_t getMemFlag(int *flagAddr)
{
	Xil_DCacheInvalidateRange((int)flagAddr, 1);
	return *flagAddr;
}

/***
 * void setMemFlag(uint32_t* flagAddr, uint8_t flagValue)
 *
 * @param uint32_t* flagAddr: pointer to the address of the shared memory flag
 * @param uint3_t   flagValue: desired memory flag value to set
 * @return none
 * @notes  none
 ***/
void setMemFlag(int *flagAddr, uint32_t flagValue)
{
	*flagAddr = flagValue;
	Xil_DCacheFlushRange((int)flagAddr, 1);
}
