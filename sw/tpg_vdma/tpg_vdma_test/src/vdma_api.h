/* @file vdma_api.h
 *
 * Adapted from:
 * https://github.com/Xilinx/embeddedsw/blob/master/XilinxProcessorIPLib/drivers/axivdma/examples/vdma_api.c
 *
 */

#ifndef VDMA_API_H
#define VDMA_API_H

#define DEBUG_MODE 0

/******************************* Include Files **************************************/
#include "xaxivdma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xil_assert.h"
#include <stdio.h>

/******************************** Custom Types **************************************/
typedef struct
{
	unsigned int      device_id; // VDMA device ID
	unsigned int      init_done; // State var; indicates if initialization is done

	unsigned int      buffer_address;        // buffer address for read and write
	unsigned int      enable_frm_cnt_intr;   // flag to trigger interrupt on frame done
	unsigned int      number_of_frame_count; // counter for VDMA frame interrupt

	XAxiVdma         *InstancePtr; // pointer to XAxiVdma instance
	XAxiVdma_DmaSetup ReadCfg;     // read channel config struct
	XAxiVdma_DmaSetup WriteCfg;    // write channel config struct

	unsigned int      hsize; // horizontal size of frame
	unsigned int      vsize; // vertical size of frame
} VdmaHandle;



/**************************** Function Definitions *********************************/

/**
 * Function to configure the read channel of the VDMA.
 *
 * @param vdma_context is the context pointer to the VDMA.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_read_setup(VdmaHandle *vdma_context);

/**
 * Function to configure the write channel of the VDMA.
 *
 * @param vdma_context is the context pointer to the VDMA.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_write_setup(VdmaHandle *vdma_context);

/**
 * Function to start VDMA transfers. If the DMA engine is operating in circular
 * buffer mode, video frames will be transferred continuously.
 *
 * @param InstancePtr is a pointer to the XAxiVdma instance.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_start_transfer(XAxiVdma *InstancePtr);

/**
 * Primary function to run VDMA. Calls the other functions defined in vdma_api.h
 * to configure and start the VDMA instance.
 *
 * @param InstancePtr is a pointer to the XAxiVdma instance.
 * @param DeviceId is the device ID of the VDMA.
 * @param hsize is the horizontal resolution of the frame.
 * @param vsize is the vertical resolution of the frame.
 * @param buf_base_addr is the address where frames will buffered in by VDMA.
 * @param number_frame_count specifies after how many frames the interrupt should trigger.
 * @param enable_frm_cnt_intr enables the frame count interrupt when set to '1'.
 *
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
int vdma_run_triple_buffer(XAxiVdma *InstancePtr, int DeviceId, int hsize, int vsize,
		                    int buf_base_addr, int number_frame_count, int enable_frm_cnt_intr);




#endif










