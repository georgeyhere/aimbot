/* @file vdma_api.c
 *
 * https://github.com/Xilinx/embeddedsw/blob/master/XilinxProcessorIPLib/drivers/axivdma/examples/vdma_api.c
 *
 */
/******************************* Include Files **************************************/
#include "vdma_api.h"


/***************************** Function Definitions *********************************/

/**
 * Function to configure the read channel of the VDMA.
 *
 * @param vdma_context is the context pointer to the VDMA.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_read_setup(VdmaHandle *vdma_context)
{
	uint32_t addr;
	int status;

	/* Initialize ReadCfg. */
	vdma_context->ReadCfg.VertSizeInput       = vdma_context->vsize;
	vdma_context->ReadCfg.HoriSizeInput       = vdma_context->hsize;
	vdma_context->ReadCfg.Stride              = vdma_context->hsize;
	vdma_context->ReadCfg.FrameDelay          = 0;
	vdma_context->ReadCfg.EnableCircularBuf   = 1;
	vdma_context->ReadCfg.EnableSync          = 1; // Gen-lock
	vdma_context->ReadCfg.PointNum            = 0;
	vdma_context->ReadCfg.EnableFrameCounter  = 0;
	vdma_context->ReadCfg.FixedFrameStoreAddr = 0;

	/* Apply ReadCfg to the VDMA instance. */
	status = XAxiVdma_DmaConfig(vdma_context->InstancePtr, XAXIVDMA_READ, &vdma_context->ReadCfg);
	if (status != XST_SUCCESS) {
		printf("VDMA read channel config failed: %d \r\n", status);
		return XST_FAILURE;
	}

	/* Initialize memory addresses for the triple frame buffers. */
	addr = vdma_context->buffer_address;
	for (int index=0; index<vdma_context->InstancePtr->MaxNumFrames; index++) {
		vdma_context->ReadCfg.FrameStoreStartAddr[index] = addr;

#if DEBUG_MODE
		{
			u8  *src;
			uint32_t total_pixel;

			total_pixel = vdma_context->stride * vdma_context->vsize;
			src = (unsigned char *) addr;
			printf("Read Buffer %d address: 0x%x \r\n", index, addr);
			for (uint32_t i; i<total_pixel; i++) {
				src[i] = i & 0xFF;
			}
		}
#endif
		addr += vdma_context->hsize * vdma_context->vsize;
	}

	/* Apply the frame buffer addresses to the VDMA instance. */
	status = XAxiVdma_DmaSetBufferAddr(vdma_context->InstancePtr, XAXIVDMA_READ,
			                           vdma_context->ReadCfg.FrameStoreStartAddr);
	if (status != XST_SUCCESS) {
		printf("VDMA read channel buffer address init failed: %d \r\n", status);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


/**
 * Function to configure the write channel of the VDMA.
 *
 * @param vdma_context is the context pointer to the VDMA.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_write_setup(VdmaHandle *vdma_context)
{
	uint32_t addr;
	int status;

	/* Initialize WriteCfg. */
	vdma_context->WriteCfg.VertSizeInput       = vdma_context->vsize;
	vdma_context->WriteCfg.HoriSizeInput       = vdma_context->hsize;
	vdma_context->WriteCfg.Stride              = vdma_context->hsize;
	vdma_context->WriteCfg.FrameDelay          = 0;
	vdma_context->WriteCfg.EnableCircularBuf   = 1;
	vdma_context->WriteCfg.EnableSync          = 0;
	vdma_context->WriteCfg.PointNum            = 0;
	vdma_context->WriteCfg.EnableFrameCounter  = 0; // endless transfers
	vdma_context->WriteCfg.FixedFrameStoreAddr = 0;

	/* Apply WriteCfg to the VDMA instance. */
	status = XAxiVdma_DmaConfig(vdma_context->InstancePtr, XAXIVDMA_WRITE, &vdma_context->WriteCfg);
	if (status != XST_SUCCESS) {
		printf("VDMA write channel config failed: %d \r\n", status);
		return XST_FAILURE;
	}

	/* Initialize memory addresses for the triple frame buffers. */
	addr = vdma_context->buffer_address;
#if DEBUG_MODE
	//
	addr = addr + vdma_context->InstancePtr->MaxNumFrames *
		  (vdma_context->stride * vdma_context->vsize);
#endif
	for(int index=0; index < vdma_context->InstancePtr->MaxNumFrames; index++) {
		vdma_context->WriteCfg.FrameStoreStartAddr[index] = addr;
#if DEBUG_MODE
		printf("Write Buffer %d address: 0x%x \r\n", index, addr);
#endif
	}

	/* Apply the frame buffer addresses to the VDMA instance. */
	status = XAxiVdma_DmaSetBufferAddr(vdma_context->InstancePtr, XAXIVDMA_WRITE,
				                       vdma_context->WriteCfg.FrameStoreStartAddr);
	if (status != XST_SUCCESS) {
		printf("VDMA write channel buffer address init failed: %d \r\n", status);
		return XST_FAILURE;
	}

#if DEBUG_MODE
	memset((void *)vdma_context->buffer_address, 0,
			vdma_context->ReadCfg.Stride * vdma_context->ReadCfg.VertSizeInput *
			vdma_context->InstancePtr->MaxNumFrames);
#endif
	return XST_SUCCESS;
}


/**
 * Function to start VDMA transfers. If the DMA engine is operating in circular
 * buffer mode, video frames will be transferred continuously.
 *
 * @param InstancePtr is a pointer to the XAxiVdma instance.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_start_transfer(XAxiVdma *InstancePtr)
{
	int status;

	/* Start the write channel of the VDMA instance. */
	status = XAxiVdma_DmaStart(InstancePtr, XAXIVDMA_WRITE);
	if (status != XST_SUCCESS) {
		printf("Write transfer start failed: %d \r\n", status);
		return XST_FAILURE;
	}

	/* Start the read channel of the VDMA instance */
	status = XAxiVdma_DmaStart(InstancePtr, XAXIVDMA_READ);
	if (status != XST_SUCCESS) {
		printf("Read transfer start failed: %d \r\n", status);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

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
		                    int buf_base_addr, int number_frame_count, int enable_frm_cnt_intr)
{
	int                   status;
	XAxiVdma_Config       *configPtr;
	XAxiVdma_FrameCounter frameCounterPtr;

	/* Initialize VDMA settings if global context_init is not set. */
	if (context_init == 0) {
		for(int i=0; i<XPAR_XAXIVDMA_NUM_INSTANCES; i++) {
			vdma_context[i].InstancePtr            = NULL;
			vdma_context[i].device_id              = -1;
			vdma_context[i].hsize                  = 0;
			vdma_context[i].vsize                  = 0;
			vdma_context[i].init_done              = 0;
			vdma_context[i].buffer_address         = 0;
			vdma_context[i].enable_frm_cnt_intr    = 0;
			vdma_context[i].number_of_frame_count  = 0;
		}
	}

	/* Look up the VDMA configuration for provided device ID. */
	configPtr = XAxiVdma_LookupConfig(DeviceId);
	if (!configPtr) {
		printf("No VDMA corresponding to ID %d \r\n", DeviceId);
		return XST_FAILURE;
	}

	/* If the VDMA instance hasn't been initialized, do so. */
	if (vdma_context[DeviceId].init_done == 0) {
		vdma_context[DeviceId].InstancePtr = InstancePtr;

		status = XAxiVdma_CfgInitialize(vdma_context[DeviceId].InstancePtr, configPtr,
				                        configPtr->BaseAddress);
		if (status != XST_SUCCESS) {
			printf("VDMA configuration initialization failed: %d \r\n", status);
			return XST_FAILURE;
		}
	}

	/* Update vdma_context members. */
	vdma_context[DeviceId].device_id             = DeviceId;
	vdma_context[DeviceId].vsize                 = vsize;
	vdma_context[DeviceId].buffer_address        = buf_base_addr;
	vdma_context[DeviceId].enable_frm_cnt_intr   = enable_frm_cnt_intr;
	vdma_context[DeviceId].number_of_frame_count = number_frame_count;
	vdma_context[DeviceId].hsize                 = hsize * (configPtr->Mm2SStreamWidth);

	/* Setup the write channel. */
	status = vdma_write_setup(&vdma_context[DeviceId]);
	if (status != XST_SUCCESS) {
		printf("Write channel setup failed: %d \r\n", status);
		if (status == XST_VDMA_MISMATCH_ERROR) {
			printf("VDMA mismatch error! \r\n");
		}
		return XST_FAILURE;
	}

	/* Setup the read channel. */
	status = vdma_read_setup(&vdma_context[DeviceId]);
	if (status != XST_SUCCESS) {
		printf("Read channel setup failed: %d \r\n", status);
		if (status == XST_VDMA_MISMATCH_ERROR) {
			printf("VDMA mismatch error! \r\n");
		}
		return XST_FAILURE;
	}

	/* Enable VDMA frame count interrupt if applicable. */
	if(vdma_context[DeviceId].enable_frm_cnt_intr) {
		frameCounterPtr.ReadDelayTimerCount  = 1;
		frameCounterPtr.ReadFrameCount       = number_frame_count;
		frameCounterPtr.WriteDelayTimerCount = 1;
		frameCounterPtr.WriteFrameCount      = number_frame_count;

		XAxiVdma_SetFrameCounter(vdma_context[DeviceId].InstancePtr, &frameCounterPtr);

		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
				            XAXIVDMA_IXR_ERROR_MASK | XAXIVDMA_IXR_FRMCNT_MASK,
				            XAXIVDMA_WRITE);
		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
				            XAXIVDMA_IXR_ERROR_MASK | XAXIVDMA_IXR_FRMCNT_MASK,
							XAXIVDMA_READ);
	}
	else {
		/* Enable only error interrupts. */
		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
				            XAXIVDMA_IXR_ERROR_MASK, XAXIVDMA_WRITE);
		XAxiVdma_IntrEnable(vdma_context[DeviceId].InstancePtr,
				            XAXIVDMA_IXR_ERROR_MASK, XAXIVDMA_READ);
	}

	/* Start VDMA transfer. */
	status = vdma_start_transfer(vdma_context[DeviceId].InstancePtr);
	if (status != XST_SUCCESS) {
		printf("VDMA transfer error: %d \r\n", status);
		if(status == XST_VDMA_MISMATCH_ERROR) {
			printf("VDMA mismatch error! \r\n");
		}
		return XST_FAILURE;
	}

#if DEBUG_MODE
	printf("Code is in Debug mode, Make sure that buffer addresses are at valid memory \r\n");
	printf("In triple mode, there has to be six consecutive buffers for Debug mode \r\n");
	{
		uint32_t addr = vdma_context[DeviceId].buffer_address;
		u8 *dst, *src;
		uint32_t total_pixel = vdma_context[DeviceId].stride * vdma_context[DeviceId].vsize;
		src = (unsigned char *)addr;
		dst = (unsigned char *)addr + (total_pixel * vdma_context->InstancePtr->MaxNumFrames);

		for (uint32_t j=0; j<vdma_context->InstancePtr->MaxNumFrames; j++) {
			for(uint32_t pixels=0; pixels<total_pixel; pixels++) {
				if (src[pixels] != dst[pixels]) {
					printf("VDMA transfer failed: SRC=0x%x, DST=0x%x \r\n", src[pixels], dst[pixels]);
					exit(-1);
				}
			}
			src = src + total_pixel;
			dst = dst + total_pixel;
		}
	}
	printf("VDMA transfer is happening and checked for 3 frames. \r\n");
#endif

	return XST_SUCCESS;
}


















