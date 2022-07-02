#include "xuartlite.h"
#include "xscugic.h"
#include "xil_printf.h"


int UartLiteInit(XUartLite *UartLiteInstPtr, u16 UartLiteDeviceId);

int UartLiteSetupIntrSystem(XScuGic *IntcInstancePtr,
				            XUartLite *UartLiteInstancePtr,
				            u16 UartLiteIntrId);