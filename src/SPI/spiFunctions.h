#include "platform.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xil_cache.h"
//
#include "xscugic.h"
#include "xspi.h"


void spiTest (XSpi* SpiInstancePtr);

int SetupSpi(XScuGic *IntcInstancePtr, u32 IntcDeviceId, XSpi *SpiInstancePtr, \
		     u16 SpiDeviceId, u16 SpiIntrId);

void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount);

void clearSpiBuffers(uint8_t* rdBuffer, uint8_t* wrBuffer, int depth);

