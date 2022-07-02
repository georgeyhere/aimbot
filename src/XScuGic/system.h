#ifndef SYSTEM_H
#define SYSTEM_H

#include "xuartlite.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xparameters.h"

#define INTC		    XScuGic
#define INTC_HANDLER	XScuGic_InterruptHandler // type of interrupt handler we're using

#define INTC_DEVICE_ID_0		  XPAR_SCUGIC_0_DEVICE_ID

/* This function configures and initializes an XScuGic instance.
*
* @param IntcInstancePtr is a pointer to the XScuGic instance.
* @param INTC_DEVICE_ID is the device ID of the XScuGic to initialize.
*
* @return XST_SUCCESS if successful, else XST_FAILURE.
*/
int SetupIntrSystem(XScuGic *IntcInstancePtr);
#endif