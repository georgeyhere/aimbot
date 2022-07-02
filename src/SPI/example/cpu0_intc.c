#include "cpu0_intc.h"

/* Configure the intc. Connect the SPI interrupt to the intc.
 *
 * @param u32 interruptDeviceID:  Interrupt controller ID
 * @param u32 interruptID:        Interrupt ID to connect
 *
 * @return XST_FAILURE if not successful; XST_SUCCESS if successful
 */
int SetupInterruptSystem(XScuGic *IntcInstancePtr, u32 interruptDeviceID)

{
	int             Status;
	XScuGic_Config *IntcConfig;

	// initialize the interrupt controller driver
	IntcConfig = XScuGic_LookupConfig(interruptDeviceID);
	if (IntcConfig == NULL) {
	   return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
	                               IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
	   return XST_FAILURE;
	}

	// initialize exception table
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
	                            (Xil_ExceptionHandler) XScuGic_InterruptHandler,
								 IntcInstancePtr);
	Xil_ExceptionEnable();
	xil_printf("Interrupt System Initialized! \r\n");

	return XST_SUCCESS;
}
