#include "spiFunctions.h"
//
/*
 * This function configures and initializes the SPI device.
 *
 * @param     SpiInstancePtr contains a pointer to the instance of the XSpi
 *            component to be configured and initialized.
 *
 * @return    XST_SUCCESS if successful, else XST_FAILURE.
 * @note      None.
 */
int SetupSpi(XScuGic *IntcInstancePtr, u32 IntcDeviceId, XSpi *SpiInstancePtr, \
		            u16 SpiDeviceId, u16 SpiIntrId)
{
	XSpi_Config *ConfigPtr;
	int Status;


    // Configure and initialize the SPI device
	ConfigPtr = XSpi_LookupConfig(SpiDeviceId);
	if(ConfigPtr == NULL){
		xil_printf("SPI Lookup Cfg failed!\n\r");
		return XST_FAILURE;
	}

	Status = XSpi_CfgInitialize(SpiInstancePtr, ConfigPtr, ConfigPtr->BaseAddress);

	// Set SPI options
	Status = XSpi_SetOptions(SpiInstancePtr,
			                 XSP_CLK_PHASE_1_OPTION | XSP_CLK_ACTIVE_LOW_OPTION);
	if(Status != XST_SUCCESS){
		xil_printf("SPI Option Config failed!\n\r");
		return XST_FAILURE;
	}

    // Run a self-test
    Status = XSpi_SelfTest(SpiInstancePtr);
    if(Status != XST_SUCCESS) {
        xil_printf("SPI Self-Test Failed!\r\n");
        return XST_FAILURE;
    }

    ///////////////
    // Connect interrupt handler
    Status = XScuGic_Connect(IntcInstancePtr, SpiIntrId,
                            (Xil_ExceptionHandler) XSpi_InterruptHandler,
                            SpiInstancePtr);
    if (Status != XST_SUCCESS) {
       return XST_FAILURE;
    }
    XScuGic_SetPriorityTriggerType(IntcInstancePtr, SpiIntrId, 0xA0, 0x3);
    XScuGic_Enable(IntcInstancePtr, SpiIntrId);
    //////////////

	// TODO: Set SPI interrupt handler
	XSpi_SetStatusHandler(SpiInstancePtr, SpiInstancePtr, 
                       (XSpi_StatusHandler)SpiHandler);

	// Start the SPI device
	XSpi_Start(SpiInstancePtr);

	// Enable SPI half-empty interrupt
	XSpi_IntrEnable(SpiInstancePtr, XSP_INTR_TX_HALF_EMPTY_MASK);


	// Initialize the exception table
	Xil_ExceptionInit();

	//Register the interrupt controller handler with the exception table
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			                     (Xil_ExceptionHandler)XScuGic_InterruptHandler,
								IntcInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

/*
 * This function is the handler which performs processing for the SPI
 * driver. It is called whenever the interrrupt is triggered.
 *
 * @param     CallBackRef is a reference passed to the handler.
 * @param     StatusEvent is the status of the SPI.
 * @param     ByteCount is the number of bytes transferred.
 *
 * @return    None.
 * @note      None.
 *
 */
void SpiHandler(void *CallBackRef, u32 StatusEvent, unsigned int ByteCount)
{
	
}

/*
 * This function clears the SPI read and write buffers.
 *
 * @param uint8_t* rdBuffer: a pointer to the read buffer to clear.
 * @param uint8_t* wrBuffer: a pointer to the write buffer to clear.
 *
 * @return: none
 * @notes:  none
 */
void clearSpiBuffers(uint8_t* rdBuffer, uint8_t* wrBuffer, int depth)
{
	for(int i=0; i<depth; i++) {
		rdBuffer[i] = 0;
		wrBuffer[i] = 0;
	}
}
