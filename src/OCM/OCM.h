#include "xil_cache.h"

#define INTC_STATUS_FLAG 0x3000001

uint32_t getMemFlag(int *flagAddr);

void    setMemFlag(int *flagAddr, uint32_t flagValue);