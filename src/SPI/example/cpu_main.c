#include <stdio.h>
#include "platform.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xil_cache.h"
//
#include "xspi.h"
#include "xscugic.h"
//
#include "cpu0_intc.h"

/************************** Constant Definitions **************************/
#define INTC_DEVICE_ID       XPAR_PS7_SCUGIC_0_DEVICE_ID
//
#define SPI_DEVICE_ID        XPAR_SPI_0_DEVICE_ID
#define SPI_IRPT_INTR        XPAR_FABRIC_SPI_0_VEC_ID
//
#define US_0_BASEADDR        XPAR_HCSR04BZ_0_S00_AXI_BASEADDR
#define US_1_BASEADDR        XPAR_HCSR04BZ_1_S00_AXI_BASEADDR
#define US_2_BASEADDR        XPAR_HCSR04BZ_2_S00_AXI_BASEADDR
//
#define BUFFER_SIZE          64

/************************** Function Prototypes **************************/
void spiTest (XSpi* SpiInstancePtr);

static int SetupSpi(XScuGic *IntcInstancePtr, u32 IntcDeviceId, XSpi *SpiInstancePtr, \
                    u16 SpiDeviceId, u16 SpiIntrId);

static void SpiHandler(void *CallBackRef, u32 StatusEvent);

void clearSpiBuffers(uint8_t* rdBuffer, uint8_t* wrBuffer);

u32 getUsDistance(u32 US_BASEADDR);

/************************** Global Variables ***************************/
static XSpi    SpiInstance;
static XScuGic IntcInstance;

u8  ReadBuffer  [BUFFER_SIZE];
u8  WriteBuffer [BUFFER_SIZE];

// indicates a transfer in progress
static volatile int TransferInProgress;


int main()
{
	int Status;
	//
	xil_printf("\r\n\nCPU0 Initialized !!!\n\r");

	// Setup scugic
	Status = SetupInterruptSystem(&SpiInstance, INTC_DEVICE_ID);
	if(Status != XST_SUCCESS) {
	    xil_printf("Interrupt system setup failed!\r\n");
	    return XST_FAILURE;
	}

	/* Setup the SPI device */
	SetupSpi(&IntcInstance, INTC_DEVICE_ID, &SpiInstance, SPI_DEVICE_ID, SPI_IRPT_INTR);
	xil_printf("CPU0 SPI initialized...\r\n\n");

	spiTest(&SpiInstance);
}


/*
 * This function runs a simple SPI test. The function waits for an
 * instruction from the Pi and sends back Hello World.
 *
 * @param XSpi *SpiInstancePtr: a pointer to the SPI instance.
 * @param uint8_t* rdBuffer:    a pointer to the SPI read buffer.
 * @param uint8_t* wrBuffer:    a pointer to the SPI write buffer.
 *
 * @return: none
 * @notes:  contains an infinite loop; may hang if no hello world
 *          command is received from the Pi.
 */
void spiTest (XSpi* SpiInstancePtr)
{
	int Status;
	int DATACOUNT;

	u32 us_val [3];

	while(1) {
		/* Send data bytes; master should send dummy bytes */
		us_val[0] = getUsDistance(US_0_BASEADDR);
		us_val[1] = getUsDistance(US_1_BASEADDR);
		us_val[2] = getUsDistance(US_2_BASEADDR);

		// Mask and load high and low bytes
		WriteBuffer[0] = us_val[0] & 0xFF;
		WriteBuffer[1] = us_val[0] & 0xFF00;
		WriteBuffer[2] = us_val[1] & 0xFF;
		WriteBuffer[3] = us_val[1] & 0xFF00;
		WriteBuffer[4] = us_val[2] & 0xFF;
		WriteBuffer[5] = us_val[2] & 0xFF00;

		// Print expected values
		DATACOUNT = 6;
		xil_printf("US0 Distance: %d cm\r\n", us_val[0]);
		xil_printf("US1 Distance: %d cm\r\n", us_val[1]);
		xil_printf("US2 Distance: %d cm\r\n", us_val[2]);
		xil_printf("\r\n");

		// Print the write buffer
		for(int i=0; i<6; i++) {
			xil_printf("WriteBuffer[%d] = %d\r\n", i, WriteBuffer[i]);
		}
		xil_printf("Waiting for master to initiate transaction...\r\n");

		TransferInProgress = TRUE;
		Status = XSpi_Transfer(&SpiInstance, // SPI device instance
				               WriteBuffer,  // buffer to write to
							   ReadBuffer,   // buffer to read to
							   DATACOUNT);   // number of bytes to send/receive
		if (Status != XST_SUCCESS) {
			xil_printf("SPI data transaction failed...\n\r");
		}
		while (TransferInProgress == TRUE);
		xil_printf("Data transaction complete.\n\n\r");

		for(int i=0; i<32; i++) {
			xil_printf("ReadBuffer[%d] = %x\r\n", i, ReadBuffer[i]);
		}
		TransferInProgress = FALSE;

		//clearSpiBuffers(ReadBuffer, WriteBuffer);
	}
}






