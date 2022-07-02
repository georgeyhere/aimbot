/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xuartlite.h"
#include "xil_exception.h"
#include <stdio.h>
#include "xscugic.h"
#include "xil_printf.h"
#include "sleep.h"
//
#include "uartUtilities.h"
#include "system.h"
#include "HCSR04Bz.h"


/************************** Constant Definitions *****************************/
#define TESTAPP_GEN

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define INTC_DEVICE_ID_0		  XPAR_SCUGIC_0_DEVICE_ID
//
#define UARTLITE_DEVICE_ID_0	  XPAR_UARTLITE_0_DEVICE_ID
#define UARTLITE_IRPT_INTR_0	  XPAR_FABRIC_UARTLITE_0_VEC_ID
//
#define UARTLITE_DEVICE_ID_1	  XPAR_UARTLITE_1_DEVICE_ID
#define UARTLITE_IRPT_INTR_1      XPAR_FABRIC_UARTLITE_1_VEC_ID
//
#define UARTLITE_DEVICE_ID_2	  XPAR_UARTLITE_2_DEVICE_ID
#define UARTLITE_IRPT_INTR_2      XPAR_FABRIC_UARTLITE_2_VEC_ID
//
#define US_BASEADDR_0             XPAR_HCSR04BZ_0_DEVICE_ID
#define US_BASEADDR_1             XPAR_HCSR04BZ_1_DEVICE_ID


/*
 * The following constant controls the length of the buffers to be sent
 * and received with the UartLite device.
 */
#define TEST_BUFFER_SIZE		100
#define N_UARTS                 3
#define N_US                    2

/**************************** Type Definitions *******************************/

#define INTC		    XScuGic
#define INTC_HANDLER	XScuGic_InterruptHandler // type of interrupt handler we're using

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

int UartLiteIntrExample(XScuGic *IntcInstancePtr,
			            XUartLite *UartLiteInstancePtr0,
						XUartLite *UartLiteInstancePtr1,
						XUartLite *UartLiteInstancePtr2,
			            u16 UartLiteIntrId_0,
						u16 UartLiteIntrId_1,
						u16 UartLiteIntrId_2);

int SetupIntrSystem(XScuGic *IntcInstancePtr);

void UartLiteRecvHandler0(void *CallBackRef, unsigned int EventData);
void UartLiteSendHandler0(void *CallBackRef, unsigned int EventData);
//
void UartLiteRecvHandler1(void *CallBackRef, unsigned int EventData);
void UartLiteSendHandler1(void *CallBackRef, unsigned int EventData);
//
void UartLiteRecvHandler2(void *CallBackRef, unsigned int EventData);
void UartLiteSendHandler2(void *CallBackRef, unsigned int EventData);


/************************** Variable Definitions *****************************/

/* Global INTC and Uart instances */
static INTC IntcInstance;	/* The instance of the Interrupt Controller */
static XUartLite UartLiteInst0;  /* UART0 */
static XUartLite UartLiteInst1;  /* UART1 */
static XUartLite UartLiteInst2;  /* UART 2 */

/* UART Send and Receive Buffers */
u8 SendBuffer[N_UARTS][TEST_BUFFER_SIZE];
u8 RecvBuffer[N_UARTS][TEST_BUFFER_SIZE];


/* Counters to determine number of bytes sent or received from each UART */
static volatile int TotalSentCount [3];
static volatile int TotalRecvCount [3];

//////////////////////////////////////////////////////////////////////////////////



int main(void)
{
    int Status;
	xil_printf("\r\nUART test start!\r\n");
	
    Status = UartLiteIntrExample(&IntcInstance,
                                 &UartLiteInst0,
                                 &UartLiteInst1,
                                 &UartLiteInst2,
                                 UARTLITE_IRPT_INTR_0,
                                 UARTLITE_IRPT_INTR_1,
                                 UARTLITE_IRPT_INTR_2);
    if(Status != XST_SUCCESS) {
        xil_printf("\r\nYou suck. Test failed.\r\n");
        return XST_FAILURE;
    }
	return XST_SUCCESS;
}


int UartLiteIntrExample(XScuGic *IntcInstancePtr,
			            XUartLite *UartLiteInstPtr_0,
                        XUartLite *UartLiteInstPtr_1,
                        XUartLite *UartLiteInstPtr_2,
			            u16 UartLiteIntrId_0,
                        u16 UartLiteIntrId_1,
                        u16 UartLiteIntrId_2)

{
	int Status;
	u32 Index;

    // Initialize UART0
    UartLiteInit(UartLiteInstPtr_0, UARTLITE_DEVICE_ID_0);
    xil_printf("UART0 Initialized!\r\n");
    UartLiteInit(UartLiteInstPtr_1, UARTLITE_DEVICE_ID_1);
    xil_printf("UART1 Initialized!\r\n");
    UartLiteInit(UartLiteInstPtr_2, UARTLITE_DEVICE_ID_2);
    xil_printf("UART2 Initialized!\r\n");
 
    Status = SetupIntrSystem(IntcInstancePtr);
    if(Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    xil_printf("XScuGic Initialized!\r\n");

    // Setup UART0 interrupts
	Status = UartLiteSetupIntrSystem(IntcInstancePtr,
					                 UartLiteInstPtr_0,
					                 UartLiteIntrId_0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	xil_printf("UART0 Interrupt Initialized!\r\n");

    // Setup UART1 interrupts
	Status = UartLiteSetupIntrSystem(IntcInstancePtr,
                                     UartLiteInstPtr_1,
                                     UartLiteIntrId_1);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    xil_printf("UART1 Interrupt Initialized!\r\n");
    
    // Setup UART2 interrupts
    Status = UartLiteSetupIntrSystem(IntcInstancePtr,
                                     UartLiteInstPtr_2,
                                     UartLiteIntrId_2);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    xil_printf("UART2 Interrupt Initialized!\r\n");

    // Set send and receive handlers
    XUartLite_SetSendHandler(UartLiteInstPtr_0, UartLiteSendHandler0, UartLiteInstPtr_0);
	XUartLite_SetRecvHandler(UartLiteInstPtr_0, UartLiteRecvHandler0, UartLiteInstPtr_0);

	XUartLite_SetSendHandler(UartLiteInstPtr_1, UartLiteSendHandler1, UartLiteInstPtr_1);
	XUartLite_SetRecvHandler(UartLiteInstPtr_1, UartLiteRecvHandler1, UartLiteInstPtr_1);

	XUartLite_SetSendHandler(UartLiteInstPtr_2, UartLiteSendHandler2, UartLiteInstPtr_2);
	XUartLite_SetRecvHandler(UartLiteInstPtr_2, UartLiteRecvHandler2, UartLiteInstPtr_2);
							 
    // Enable the UART device interrupts
	XUartLite_EnableInterrupt(UartLiteInstPtr_0);
    XUartLite_EnableInterrupt(UartLiteInstPtr_1);
    XUartLite_EnableInterrupt(UartLiteInstPtr_2);

	int   Timer;
	_Bool Valid  [N_UARTS];
	int   Errors [N_UARTS];
    int   DATACOUNT [N_UARTS];
	char  c;
	const char * text;
    u32 us_distance [N_US];
    u8 tempBuffer [20];
    
    // Main Loop
	while(1) {
        
        /*
        text = "UART0: Long: 34.05825 | Lat: -117.82188\r\n";
		DATACOUNT[0] = 0;
		for (text; c = *text; text++) {
			SendBuffer[0][DATACOUNT[0]] = c;
			xil_printf("SendBuffer0[%d] = %c\r\n", DATACOUNT[0], SendBuffer[0][DATACOUNT[0]]);
			DATACOUNT[0]++;
		}
        */
        DATACOUNT[0] = 0;
        us_distance[0] = getUsDistance(US_BASEADDR_0);
        us_distance[1] = getUsDistance(US_BASEADDR_1);
        //
        *tempBuffer = (u8*) us_distance[0];
        for(int i=0; i<3; i++) {
            SendBuffer[0][i] = tempBuffer[i];
        }
        //
        *tempBuffer = (u8*) us_distance[1];
        for(int i=0; i<3; i++) {
            SendBuffer[0][i+3] = tempBuffer[i];
        }

		text = "UART1: Drive Motor (RPM): 0 | Steering Angle (deg): 90\r\n";
		DATACOUNT[1] = 0;
		for (text; c = *text; text++) {
			SendBuffer[1][DATACOUNT[1]] = c;
			xil_printf("SendBuffer1[%d] = %c\r\n", DATACOUNT[1], SendBuffer[1][DATACOUNT[1]]);
			DATACOUNT[1]++;
		}

        text = "UART2: Toucan MGM[0] Angle (deg): 0 | Toucan MGM[1] Angle (deg): 0 | Toucan MGM[2] Angle (deg): 0\r\n";
		DATACOUNT[2] = 0;
		for (text; c = *text; text++) {
			SendBuffer[2][DATACOUNT[2]] = c;
			xil_printf("SendBuffer2[%d] = %c\r\n", DATACOUNT[2], SendBuffer[2][DATACOUNT[2]]);
			DATACOUNT[2]++;
		}

        /* Trigger UART sends */
        XUartLite_Send(UartLiteInstPtr_0, SendBuffer[0], DATACOUNT[0]);
        XUartLite_Send(UartLiteInstPtr_1, SendBuffer[1], DATACOUNT[1]);
        XUartLite_Send(UartLiteInstPtr_2, SendBuffer[2], DATACOUNT[2]);

        /* Hang until UARTS are done sending */
		xil_printf("Sending...\r\n");
        for(int i=0; i<N_UARTS; i++) {
            TotalSentCount[i] = 0;
            TotalRecvCount[i] = 0;
        }
		while ( (TotalSentCount[0] != DATACOUNT[0]) &&
                (TotalSentCount[1] != DATACOUNT[1]) &&
                (TotalSentCount[2] != DATACOUNT[2])) {
            // !!! app will hang here if UART interrupts are not triggered !!!
        }

        /* Wait for Pi to prepare its send */
		sleep(0.1);
		
        /* Receive data from the Pi */
		xil_printf("Receiving...\r\n");
		for(int i=0; i<3; i++) {
            Valid[i] = TRUE;
        }
		XUartLite_Recv(UartLiteInstPtr_0, RecvBuffer[0], DATACOUNT[0]);
        XUartLite_Recv(UartLiteInstPtr_1, RecvBuffer[1], DATACOUNT[1]);
        XUartLite_Recv(UartLiteInstPtr_2, RecvBuffer[2], DATACOUNT[2]);

        /* Wait for UARTs to be done */
		while ( (TotalRecvCount[0] != DATACOUNT[0]) &&
                (TotalRecvCount[1] != DATACOUNT[1]) &&
                (TotalRecvCount[2] != DATACOUNT[2])) {
            
            // Receive timeout
            Timer++;
			if(Timer == 1000000) {
				xil_printf("Receive timed out...\r\n");
				for(int i=0; i<3; i++) {
                    Errors[i] = DATACOUNT[i] - TotalRecvCount[i];
                    if(Errors[i] > 0) {
                        xil_printf("ERROR: UART%d still had %d bytes to send...\r\n", i, Errors[i]);
                        Valid[i] = FALSE;
                    }
                }
				break;
			}

            // Check overrun errors
			Errors[0] = UartLiteInstPtr_0->Stats.ReceiveOverrunErrors;
            Errors[1] = UartLiteInstPtr_1->Stats.ReceiveOverrunErrors;
            Errors[2] = UartLiteInstPtr_2->Stats.ReceiveOverrunErrors;
            for(int i=0; i<3; i++) {
                if(Errors[i] > 0) {
                    xil_printf("\r\nERROR: UART%d has %d RX Overrun Errors... \r\n\n", i, Errors[i]);
                    Valid[i] = FALSE;
                }
            }
		}
		Timer = 0;

        
		/* Print valid RX data */
		for(int i=0; i<3; i++) {
            if(Valid[i]) {
                switch (i) {
                    case 0:
                       for(int j=0; j<DATACOUNT[i]; j++) {
                           xil_printf("UART0 RecvBuffer%d[%d] : %s\r\n", i, j, RecvBuffer[0][j]);
                       }
                       break;
                    case 1:
                       for(int j=0; j<DATACOUNT[i]; j++) {
                           xil_printf("UART1 RecvBuffer%d[%d] : %s\r\n", i, j, RecvBuffer[1][j]);
                       }
                       break;
                    case 2:
                       for(int j=0; j<DATACOUNT[i]; j++) {
                           xil_printf("UART2 RecvBuffer%d[%d] : %s\r\n", i, j, RecvBuffer[2][j]);
                       }
                       break;
                }
            }
            else {
                xil_printf("UART%d transaction failed!\r\n", i);
            }
        }
        
        /* Reset receive counters */
		for(int i=0; i<3; i++) {
            TotalRecvCount[i] = 0;
        }

		sleep(3);
    }
	return XST_SUCCESS;
}

/**
 * @brief Uartlite send interrupt handler for UARTLITE0.
 * 
 * @param CallBackRef CallBackRef contains a callback reference from the driver. 
 *                    It should be the instance pointer for a Uartlite driver.
 * @param EventData   EventData contains the number of bytes sent for send events.
 */
void UartLiteSendHandler0(void *CallBackRef, unsigned int EventData) {
	TotalSentCount[0] = EventData;
}

/**
 * @brief Uartlite send interrupt handler for UARTLITE0.
 * 
 * @param CallBackRef CallBackRef contains a callback reference from the driver. 
 *                    It should be the instance pointer for a Uartlite driver.
 * @param EventData   EventData contains the number of bytes sent for receive events.
 */
void UartLiteRecvHandler0(void *CallBackRef, unsigned int EventData) {
	TotalRecvCount[0] = EventData;
}

/**
 * @brief Uartlite send interrupt handler for UARTLITE1.
 * 
 * @param CallBackRef CallBackRef contains a callback reference from the driver. 
 *                    It should be the instance pointer for a Uartlite driver.
 * @param EventData   EventData contains the number of bytes sent for send events.
 */
void UartLiteSendHandler1(void *CallBackRef, unsigned int EventData) {
	TotalSentCount[1] = EventData;
}

/**
 * @brief Uartlite send interrupt handler for UARTLITE1.
 * 
 * @param CallBackRef CallBackRef contains a callback reference from the driver. 
 *                    It should be the instance pointer for a Uartlite driver.
 * @param EventData   EventData contains the number of bytes sent for send events.
 */
void UartLiteRecvHandler1(void *CallBackRef, unsigned int EventData) {
	TotalRecvCount[1] = EventData;
}

/**
 * @brief Uartlite send interrupt handler for UARTLITE2.
 * 
 * @param CallBackRef CallBackRef contains a callback reference from the driver. 
 *                    It should be the instance pointer for a Uartlite driver.
 * @param EventData   EventData contains the number of bytes sent for send events.
 */
void UartLiteSendHandler2(void *CallBackRef, unsigned int EventData) {
	TotalSentCount[2] = EventData;
}

/**
 * @brief Uartlite send interrupt handler for UARTLITE2.
 * 
 * @param CallBackRef CallBackRef contains a callback reference from the driver. 
 *                    It should be the instance pointer for a Uartlite driver.
 * @param EventData   EventData contains the number of bytes sent for send events.
 */
void UartLiteRecvHandler2(void *CallBackRef, unsigned int EventData) {
	TotalRecvCount[2] = EventData;
}