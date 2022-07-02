#include "uartUtilities.h"





/* This function connects the UartLite interrupt to an XScuGic object.
*  It requires the XScuGic to already be configured and initialized.
*
*  The UART interrupt handlers and device interrupt STILL NEED to
*  be set outside of this function before proper operation!!!!
* 
*  @param IntcInstancePtr is a pointer to the XScuGic instance.
*  @param UartLiteInstPtr is a pointer to the UartLite instance.
*  @param UartLiteIntrId is the ID of the interrupt to connect to the XScuGic.
*
*  @return XST_SUCCESS if successful, else the error from XScuGic_Connect.
*/
int UartLiteSetupIntrSystem(XScuGic *IntcInstancePtr,
				            XUartLite *UartLiteInstPtr,
				            u16 UartLiteIntrId)
{
	int Status;

    // Set UART interrupt priority
	XScuGic_SetPriorityTriggerType(IntcInstancePtr, UartLiteIntrId,
					0xA0, 0x3);

    // Connect interrupts
	Status = XScuGic_Connect(IntcInstancePtr, UartLiteIntrId,
				 (Xil_ExceptionHandler)XUartLite_InterruptHandler,
				 UartLiteInstPtr);
	if (Status != XST_SUCCESS) {
        xil_printf("UART Interrupt xscugic connection failed!\r\n");
		return Status;
	}

    // Enable the UART interrupt
	XScuGic_Enable(IntcInstancePtr, UartLiteIntrId);

	return XST_SUCCESS;
}

/* This function initializes and configures an UartLite instance.
*  
*  @param UartLiteInstPtr is a pointer to the UartLite instance.
*  @param UartLiteDeviceId is the device ID of the UartLite.
*
*  @return XST_SUCCESS if successful, else XST_FAILURE.
*/
int UartLiteInit(XUartLite *UartLiteInstPtr, u16 UartLiteDeviceId)
{
    int Status;
    Status = XUartLite_Initialize(UartLiteInstPtr, UartLiteDeviceId);
	if (Status != XST_SUCCESS) {
        xil_printf("UART initialization failed! Device ID: %x\r\n", UartLiteDeviceId);
		return XST_FAILURE;
	}

	Status = XUartLite_SelfTest(UartLiteInstPtr);
	if (Status != XST_SUCCESS) {
        xil_printf("UART self test failed! Device ID: %d\r\n", UartLiteDeviceId);
		return XST_FAILURE;
	}

    return XST_SUCCESS;
}