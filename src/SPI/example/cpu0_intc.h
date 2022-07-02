#include "platform.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xil_cache.h"
//
#include "xscugic.h"

int SetupInterruptSystem(XScuGic *IntcInstancePtr, u32 interruptDeviceID);

