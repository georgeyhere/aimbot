
#include "system.h"

/**
 * @brief A function to configure and initialize an xscugic instance.
 * 
 * @param IntcInstancePtr is a pointer to the xscugic driver instance.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
int SetupIntrSystem(XScuGic *IntcInstancePtr)
{
    int Status;
    XScuGic_Config *IntcConfig;

    // Lookup scugic config
    IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID_0);
    if(IntcConfig == NULL) {
        xil_printf("Could not find INTC to configure!\r\n");
        return XST_FAILURE;
    }

    // Initialize scugic
    Status = XScuGic_CfgInitialize(IntcInstancePtr, 
                                   IntcConfig,
                                   IntcConfig -> CpuBaseAddress);
    if(Status != XST_SUCCESS) {
        xil_printf("INTC config initialization failed!\r\n");
        return XST_FAILURE;
    }

    // Enable exceptions
    Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)INTC_HANDLER,
			IntcInstancePtr);

	Xil_ExceptionEnable();
    return XST_SUCCESS;
}
